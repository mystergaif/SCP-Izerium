#include "raylib.h"
#include "raymath.h"    // Для Vector3, Quaternion, Matrix
#include <vector>       // Для std::vector
#include <cstdlib>      // Для rand(), srand()
#include <ctime>        // Для time()

// --- Включаем заголовки Bullet Physics ---
#include "bullet3/src/LinearMath/btDefaultMotionState.h"
#include "bullet3/src/btBulletDynamicsCommon.h"
#include "bullet3/src/BulletCollision/CollisionShapes/btBoxShape.h"


// --- Константы игры ---
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define CUBE_SIZE 1.0f // Размер стороны куба
#define PLATFORM_THICKNESS 1.0f // Толщина платформы для физики
#define CUBE_SPAWN_HEIGHT 10.0f
#define MAX_CUBES 50
#define GRAB_DISTANCE 5.0f // Дистанция, на которой можно схватить куб
#define THROW_FORCE 6.0f  // <-- Изменено: Сила броска
#define GRAVITY_MAGNITUDE 9.8f // Сила гравитации

// --- Структуры данных ---
typedef struct MovableCube {
    Vector3 visualPosition;   // Позиция для рендера (из Bullet)
    Quaternion visualRotation; // Вращение для рендера (из Bullet)
    btRigidBody* physicsBody; // Указатель на жесткое тело Bullet
    bool isGrabbed;
    Color color;              // <-- Добавлено: Цвет куба
} MovableCube;

typedef struct Platform {
    Vector3 visualPosition;
    Quaternion visualRotation;
    Vector3 size; // Для физики и отрисовки (как куб)
    btRigidBody* physicsBody;
} Platform;

// --- Глобальные переменные для Bullet Physics ---
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btBroadphaseInterface* overlappingPairCache;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld; // Ваш основной физический мир

// Контейнеры для управления объектами Bullet (важно для очистки памяти)
std::vector<btCollisionShape*> collisionShapes;
std::vector<btRigidBody*> rigidBodies;

// --- Глобальные переменные игры ---
MovableCube movableCubes[MAX_CUBES];
int numCubes = 0;
Platform mainPlatform;
Camera3D camera = { 0 };

Texture2D grassTexture; // Общая текстура для всех объектов
Model cubeModel; // Модель для всех кубов (программно созданная)
Model platformModel; // Модель для платформы (программно созданная)

// Переменные для захвата/броска
int grabbedCubeIndex = -1;
bool isGrabbing = false;

// Для прицела
bool isLookingAtGrababbleObject = false; // Флаг, указывающий, смотрим ли мы на схватываемый объект

// --- Функции Bullet Physics ---

void InitBulletPhysics() {
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -GRAVITY_MAGNITUDE, 0));
}

void UpdateBulletPhysics(float deltaTime) {
    dynamicsWorld->stepSimulation(deltaTime, 10);
}

void CloseBulletPhysics() {
    // Удаляем все RigidBody из мира
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    // Удаляем формы столкновения
    for (size_t i = 0; i < collisionShapes.size(); i++) {
        delete collisionShapes[i];
    }
    collisionShapes.clear();

    // Удаляем остальные объекты Bullet (основные компоненты мира)
    delete dynamicsWorld;
    delete solver;
    delete overlappingPairCache;
    delete dispatcher;
    delete collisionConfiguration;

    rigidBodies.clear(); // Очищаем вектор указателей
}

// Создает жесткое тело Bullet (для кубов и платформы)
btRigidBody* CreateBulletBody(Vector3 position, Vector3 size, float mass, float friction, float restitution) {
    btCollisionShape* colShape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
    collisionShapes.push_back(colShape);

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(position.x, position.y, position.z));

    bool isDynamic = (mass != 0.0f);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic) {
        colShape->calculateLocalInertia(mass, localInertia);
    }

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setFriction(friction);
    body->setRestitution(restitution);

    dynamicsWorld->addRigidBody(body);
    rigidBodies.push_back(body);

    return body;
}

// --- Функции игры ---

void AddCube(Vector3 position) {
    if (numCubes < MAX_CUBES) {
        movableCubes[numCubes].physicsBody = CreateBulletBody(position, (Vector3){CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, 1.0f, 0.5f, 0.2f);
        movableCubes[numCubes].isGrabbed = false;
        
        // <-- Добавлено: Случайный цвет для куба
        movableCubes[numCubes].color = (Color){ (unsigned char)GetRandomValue(50, 250), (unsigned char)GetRandomValue(50, 250), (unsigned char)GetRandomValue(50, 250), 255 };
        
        numCubes++;
    }
}

void InitGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SCP Game - Bullet Physics (Procedural)");
    SetTargetFPS(60);
    srand(time(NULL)); // Инициализация генератора случайных чисел

    DisableCursor();

    camera.position = (Vector3){ 0.0f, 2.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    grassTexture = LoadTexture("grass.png");

    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    cubeModel = LoadModelFromMesh(cubeMesh);
    // Исключаем применение текстуры к кубам, чтобы они могли быть разных цветов
    // SetMaterialTexture(&cubeModel.materials[0], MATERIAL_MAP_DIFFUSE, grassTexture);

    platformModel = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    SetMaterialTexture(&platformModel.materials[0], MATERIAL_MAP_DIFFUSE, grassTexture);


    InitBulletPhysics();

    mainPlatform.size = (Vector3){20.0f, PLATFORM_THICKNESS, 20.0f};
    mainPlatform.physicsBody = CreateBulletBody((Vector3){0, -PLATFORM_THICKNESS * 0.5f, 0}, mainPlatform.size, 0.0f, 0.8f, 0.0f);

    AddCube((Vector3){0.0f, CUBE_SPAWN_HEIGHT, 0.0f});
    AddCube((Vector3){0.5f, CUBE_SPAWN_HEIGHT + 2.0f, 0.5f});
    AddCube((Vector3){-0.5f, CUBE_SPAWN_HEIGHT + 4.0f, -0.5f});
}

void UpdateGame() {
    float deltaTime = GetFrameTime();

    UpdateCamera(&camera, CAMERA_FIRST_PERSON);

    UpdateBulletPhysics(deltaTime);

    // --- Проверка наведения на схватываемый объект для прицела ---
    isLookingAtGrababbleObject = false;
    Ray centerRay = GetMouseRay((Vector2){(float)GetScreenWidth()/2, (float)GetScreenHeight()/2}, camera);
    
    btVector3 rayFrom = btVector3(camera.position.x, camera.position.y, camera.position.z);
    btVector3 rayTo = btVector3(centerRay.direction.x * GRAB_DISTANCE, centerRay.direction.y * GRAB_DISTANCE, centerRay.direction.z * GRAB_DISTANCE);
    rayTo += rayFrom;
    
    btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
    dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);

    if (rayCallback.hasHit()) {
        btRigidBody* hitBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
        if (hitBody && hitBody->getMass() > 0.0f) {
            for (int i = 0; i < numCubes; i++) {
                if (movableCubes[i].physicsBody == hitBody) {
                    Vector3 hitPos = (Vector3){rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z()};
                    if (Vector3Distance(camera.position, hitPos) <= GRAB_DISTANCE + CUBE_SIZE * 0.5f)
                    {
                        isLookingAtGrababbleObject = true;
                    }
                    break;
                }
            }
        }
    }


    // --- Логика захвата/броска куба ---
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (!isGrabbing) {
            if (rayCallback.hasHit()) {
                btRigidBody* hitBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
                if (hitBody && hitBody->getMass() > 0.0f) {
                    for (int i = 0; i < numCubes; i++) {
                        if (movableCubes[i].physicsBody == hitBody) {
                            Vector3 hitPos = (Vector3){rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z()};
                            if (Vector3Distance(camera.position, hitPos) <= GRAB_DISTANCE + CUBE_SIZE * 0.5f) {
                                grabbedCubeIndex = i;
                                isGrabbing = true;
                                movableCubes[i].isGrabbed = true;
                                
                                hitBody->setGravity(btVector3(0,0,0));
                                hitBody->setLinearFactor(btVector3(0,0,0));
                                hitBody->setAngularFactor(btVector3(0,0,0));
                                hitBody->setActivationState(DISABLE_DEACTIVATION);
                            }
                            break;
                        }
                    }
                }
            }
        } else {
            isGrabbing = false;
            movableCubes[grabbedCubeIndex].isGrabbed = false;
            
            btRigidBody* body = movableCubes[grabbedCubeIndex].physicsBody;
            
            body->setGravity(btVector3(0, -GRAVITY_MAGNITUDE, 0));
            body->setLinearFactor(btVector3(1,1,1));
            body->setAngularFactor(btVector3(1,1,1));
            body->setActivationState(ACTIVE_TAG);

            Vector3 throwDirection = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
            
            body->applyImpulse(btVector3(throwDirection.x * THROW_FORCE, throwDirection.y * THROW_FORCE, throwDirection.z * THROW_FORCE), btVector3(0, 0, 0));
            
            grabbedCubeIndex = -1;
        }
    }

    // --- Логика спавна куба по правой кнопке мыши ---
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        if (numCubes < MAX_CUBES) {
            Vector3 spawnPos = Vector3Add(camera.position, Vector3Scale(Vector3Normalize(Vector3Subtract(camera.target, camera.position)), GRAB_DISTANCE * 0.8f));
            AddCube(spawnPos);
        }
    }

    // Если куб схвачен, перемещаем его перед камерой
    if (isGrabbing && grabbedCubeIndex != -1) {
        btRigidBody* body = movableCubes[grabbedCubeIndex].physicsBody;
        
        Vector3 desiredPos = Vector3Add(camera.position, Vector3Scale(Vector3Normalize(Vector3Subtract(camera.target, camera.position)), GRAB_DISTANCE * 0.8f));
        
        btTransform currentTrans;
        body->getMotionState()->getWorldTransform(currentTrans);
        currentTrans.setOrigin(btVector3(desiredPos.x, desiredPos.y, desiredPos.z));
        body->setWorldTransform(currentTrans);
    }

    // Обновляем визуальные позиции и вращения кубов из физического мира
    for (int i = 0; i < numCubes; i++) {
        btTransform trans;
        movableCubes[i].physicsBody->getMotionState()->getWorldTransform(trans);

        btVector3 bulletPos = trans.getOrigin();
        btQuaternion bulletRot = trans.getRotation();

        movableCubes[i].visualPosition = (Vector3){ bulletPos.x(), bulletPos.y(), bulletPos.z() };
        movableCubes[i].visualRotation = (Quaternion){ bulletRot.x(), bulletRot.y(), bulletRot.z(), bulletRot.w() };
    }
    
    // Обновляем визуальные позиции и вращения платформы
    btTransform platformTrans;
    mainPlatform.physicsBody->getMotionState()->getWorldTransform(platformTrans);
    btVector3 platformPos = platformTrans.getOrigin();
    btQuaternion platformRot = platformTrans.getRotation();

    mainPlatform.visualPosition = (Vector3){ platformPos.x(), platformPos.y(), platformPos.z() };
    mainPlatform.visualRotation = (Quaternion){ platformRot.x(), platformRot.y(), platformRot.z(), platformRot.w() };
}

void DrawGame() {
    BeginDrawing();
    ClearBackground(SKYBLUE);

    BeginMode3D(camera);
        // Рисуем платформу
        Vector3 pAxis;
        float pAngle;
        QuaternionToAxisAngle(mainPlatform.visualRotation, &pAxis, &pAngle);
        // Масштабируем платформу по ее размерам
        DrawModelEx(platformModel, mainPlatform.visualPosition, pAxis, pAngle * RAD2DEG, mainPlatform.size, WHITE);


        // Рисуем кубы
        for (int i = 0; i < numCubes; i++) {
            Vector3 cAxis;
            float cAngle;
            QuaternionToAxisAngle(movableCubes[i].visualRotation, &cAxis, &cAngle);
            // Используем color, заданный для каждого куба
            DrawModelEx(cubeModel, movableCubes[i].visualPosition, cAxis, cAngle * RAD2DEG, (Vector3){CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, movableCubes[i].color);
        }

        DrawGrid(10, 1.0f);
    EndMode3D();

    // --- Рисуем крестик прицела (в 2D-режиме, поверх 3D-сцены) ---
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;
    int crosshairSize = 10;
    int crosshairGap = 2; // Разрыв в центре крестика
    Color crosshairColor = WHITE;

    if (isLookingAtGrababbleObject) {
        crosshairColor = GREEN; // Если смотрим на схватываемый объект, делаем крестик зеленым
    }

    // Горизонтальные линии
    DrawRectangle(centerX - crosshairSize - crosshairGap, centerY - 1, crosshairSize, 2, crosshairColor); // Левая
    DrawRectangle(centerX + crosshairGap, centerY - 1, crosshairSize, 2, crosshairColor); // Правая
    // Вертикальные линии
    DrawRectangle(centerX - 1, centerY - crosshairSize - crosshairGap, 2, crosshairSize, crosshairColor); // Верхняя
    DrawRectangle(centerX - 1, centerY + crosshairGap, 2, crosshairSize, crosshairColor); // Нижняя

    DrawFPS(10, 10);
    EndDrawing();
}

void CloseGame() {
    UnloadTexture(grassTexture);
    UnloadModel(cubeModel);
    UnloadModel(platformModel);

    CloseBulletPhysics();

    EnableCursor();
    CloseWindow();
}

int main() {
    InitGame();

    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }

    CloseGame();

    return 0;
}
