/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute 'raylib_compile_execute' script
*   Note that compiled executable is placed in the same folder as .c file
*
*   To test the examples on Web, press F6 and execute 'raylib_compile_execute_web' script
*   Web version of the program is generated in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "raymath.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1800;
    const int screenHeight = 900;
    const float playerMoveSpeed = 0.25f;
    const int MaxPlayerTankBullets = 20;
    const int MaxPlayerMGBullets = 70;
    const int MaxEnemyTankBullets = 30;
    const int MaxEnemyMGBullets = 70;
    const int PlayerTankDelay = 1;
    const float PlayerMGDelay = 0.1f;
    const int MaxNumberOfEnemyTanks = 20;
    const int MaxNumberOfEnemyAPCs = 15;
    const int MaxNumberOfSpecialBullets = 30;
    const int MaxNumberOfBattleShipTankBullets = 30;
    const int MaxPlayerMainGunAmmo = 50;
    const int MaxPlayerMGAmmo = 300;
    const int HealthBoost = 75;
    const int MainGunAmmoBoost = 30;
    const int MGAmmoBoost = 60;
    const int MaxNumberOfPickups = 52;
    const int VerticalWallCount = 54;
    const int HorizontalWallCount = 48;
    const int Building1Count = 40;
    const int MaxBattleshipHealth = 500;
    const int SpecialBulletDamage = 50;
    const float BattleshipFireRate = 1;
    
    Image GameIcon = LoadImage("The Last Tank/GameIcon.png");
    InitWindow(screenWidth, screenHeight, "The Last Tank - A Game By Akshat Maurya");
    SetWindowIcon(GameIcon);
    InitAudioDevice();
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    //models
    Model playerTank = LoadModel("The Last Tank/PlayerTank.obj");                   //setting up and loading model
    Model tankBullet = LoadModel("The Last Tank/TankBullet.obj");
    Model EnemyTankModel = LoadModel("The Last Tank/EnemyTank.obj");
    Model EnemyAPCModel = LoadModel("The Last Tank/EnemyAPC.obj");
    Model MGBullet = LoadModel("The Last Tank/GunBullet.obj");
    Model HealthPickup = LoadModel("The Last Tank/HealthPickup.obj");
    Model MainGunPickup = LoadModel("The Last Tank/MainGunPickup.obj");
    Model MGPickup = LoadModel("The Last Tank/MGPickup.obj");
    Model Wall_Horizontal = LoadModel("The Last Tank/HorizontalWallSegment.obj");
    Model Wall_Vertical = LoadModel("The Last Tank/VerticalWallSegment.obj");
    Model Building1 = LoadModel("The Last Tank/Building1.obj");
    Model LevelModel = LoadModel("The Last Tank/Level.obj");
    Model BattleShipModel = LoadModel("The Last Tank/LandBattleship.obj");
    Model BigBullet = LoadModel("The Last Tank/BigBullet.obj");
    
    //textures
    Texture2D playerTank_tex = LoadTexture("The Last Tank/PlayerTank_BaseColor.png");     //loading texture
    Texture2D bulletTexture = LoadTexture("The Last Tank/Bullets.png");
    Texture2D enemyTank_tex = LoadTexture("The Last Tank/EnemyTank_BaseColor.png");
    Texture2D enemyAPC_tex = LoadTexture("The Last Tank/EnemyTank_BaseColor.png");
    Texture2D pickupTexture = LoadTexture("The Last Tank/Pickups_BaseColor.png");
    Texture2D building1_tex = LoadTexture("The Last Tank/Building1_BaseColor.png");
    Texture2D level_tex = LoadTexture("The Last Tank/Level_BaseColor.png");
    Texture2D battleship_tex = LoadTexture("The Last Tank/LandBattleship_BaseColor.png");
    Texture2D healthIcon_tex = LoadTexture("The Last Tank/HealthIcon.png");
    Texture2D MainGunIcon_tex = LoadTexture("The Last Tank/MainAmmoIcon.png");
    Texture2D MGIcon_tex = LoadTexture("The Last Tank/MGAmmoIcon.png");
    Texture2D explosionFlipBookTexture = LoadTexture("The Last Tank/Explosion00_5x5.png");
    
    //materials
    playerTank.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = playerTank_tex; //assigning texture to model
    tankBullet.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = bulletTexture;
    MGBullet.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = bulletTexture;
    EnemyTankModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = enemyTank_tex;
    EnemyAPCModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = enemyAPC_tex;
    HealthPickup.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = pickupTexture;
    MainGunPickup.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = pickupTexture;
    MGPickup.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = pickupTexture;
    Building1.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = building1_tex;
    LevelModel.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = level_tex;
    LevelModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = building1_tex;
    BattleShipModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = battleship_tex;
    BigBullet.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = bulletTexture;
    
    //audio
    Sound PlayerTankGunSound = LoadSound("The Last Tank/TLT_explosion_6.wav");
    Sound PlayerMGSound = LoadSound("The Last Tank/TLT_machine_gun.wav");
    Sound EnemyDieSound = LoadSound("The Last Tank/TLT_explosion_7.wav");
    Sound EnemyHitSound = LoadSound("The Last Tank/TLT_hit_1.wav");
    Sound EnemyTankGunSound = LoadSound("The Last Tank/TLT_explosion_1.wav");
    Sound HealthPickupSound = LoadSound("The Last Tank/TLT_health.wav");
    
    Camera cam = {0};                                             //setting up camera
    cam.position = (Vector3){0.0f, 16.0f, 8.0f};
    cam.target = (Vector3){0.0f, 0.0f, 0.0f};
    cam.fovy = 90.0f;
    cam.up = (Vector3){0.0f, 1.0f, 0.0f};
    cam.projection = CAMERA_PERSPECTIVE;
    
    //walls data
    Vector3 VerticalWallPositions[] = { (Vector3){-25.0f, 0.0f, 1.0f}, (Vector3){-25.0f, 0.0f, -9.0f}, 
                                                        (Vector3){-25.0f, 0.0f, -19.0f}, (Vector3){-25, 0.0f, -39.0f},
                                                        (Vector3){-25.0f, 0.0f, -29.0f}, (Vector3){25.0f, 0.0f, -29.0f},
                                                        (Vector3){25.0f, 0.0f, 1.0f}, (Vector3){25.0f, 0.0f, -9.0f},
                                                        (Vector3){25.0f, 0.0f, -19.0f}, (Vector3){25.0f, 0.0f, -39.0f},
                                                        (Vector3){25.0f, 0.0f, -49.0f}, (Vector3){25.0f, 0.0f, -59.0f},
                                                        (Vector3){25.0f, 0.0f, -69.0f}, (Vector3){25.0f, 0.0f, -79.0f},
                                                        (Vector3){25.0f, 0.0f, -89.0f}, (Vector3){25.0f, 0.0f, -99.0f},
                                                        (Vector3){25.0f, 0.0f, -109.0f}, (Vector3){5.0f, 0.0f, -49.0f},
                                                        (Vector3){-25.0f, 0.0f, -59.0f}, (Vector3){-25.0f, 0.0f, -69.0f},
                                                        (Vector3){-25.0f, 0.0f, -79.0f}, (Vector3){-25.0f, 0.0f, -89.0f},
                                                        (Vector3){-25.0f, 0.0f, -99.0f}, (Vector3){5.0f, 0.0f, -109.0f},
                                                        (Vector3){-25.0f, 0.0f, -119.0f}, (Vector3){-25.0f, 0.0f, -129.0f},
                                                        (Vector3){-25.0f, 0.0f, -139.0f}, (Vector3){-25.0f, 0.0f, -149.0f},
                                                        (Vector3){-25.0f, 0.0f, -159.0f}, (Vector3){-25.0f, 0.0f, -169.0f},
                                                        (Vector3){-25.0f, 0.0f, -179.0f}, (Vector3){55.0f, 0.0f, -119.0f},
                                                        (Vector3){55.0f, 0.0f, -129.0f}, (Vector3){55.0f, 0.0f, -139.0f},
                                                        (Vector3){55.0f, 0.0f, -149.0f}, (Vector3){55.0f, 0.0f, -159.0f},
                                                        (Vector3){55.0f, 0.0f, -169.0f}, (Vector3){55.0f, 0.0f, -179.0f},
                                                        (Vector3){5.0f, 0.0f, -189.0f}, (Vector3){25.0f, 0.0f, -189.0f}, 
                                                        (Vector3){75.0f, 0.0f, -199.0f}, (Vector3){75.0f, 0.0f, -209.0f},
                                                        (Vector3){75.0f, 0.0f, -219.0f}, (Vector3){75.0f, 0.0f, -229.0f},
                                                        (Vector3){75.0f, 0.0f, -239.0f}, (Vector3){75.0f, 0.0f, -249.0f},
                                                        (Vector3){75.0f, 0.0f, -259.0f}, (Vector3){-45.0f, 0.0f, -199.0f},
                                                        (Vector3){-45.0f, 0.0f, -209.0f}, (Vector3){-45.0f, 0.0f, -219.0f},
                                                        (Vector3){-45.0f, 0.0f, -229.0f}, (Vector3){-45.0f, 0.0f, -239.0f},
                                                        (Vector3){-45.0f, 0.0f, -249.0f}, (Vector3){-45.0f, 0.0f, -259.0f} };
    
    Vector3 HorizontalWallPositions[] = { (Vector3){-20.0f, 0.0f, 6.0f}, (Vector3){-10.0f, 0.0f, 6.0f}, 
                                                            (Vector3){0.0f, 0.0f, 6.0f}, (Vector3){10.0f, 0.0f, 6.0f},
                                                            (Vector3){20.0f, 0.0f, 6.0f}, (Vector3){-20.0f, 0.0f, -44.0f},
                                                            (Vector3){-10.0f, 0.0f, -44.0f}, (Vector3){0.0f, 0.0f, -44.0f},
                                                            (Vector3){-20.0f, 0.0f, -54.0f}, (Vector3){-10.0f, 0.0f, -54.0f},
                                                            (Vector3){0.0f, 0.0f, -54.0f}, (Vector3){-20.0f, 0.0f, -104.0f}, 
                                                            (Vector3){-10.0f, 0.0f, -104.0f}, (Vector3){0.0f, 0.0f, -104.0f},
                                                            (Vector3){-20.0f, 0.0f, -114.0f}, (Vector3){-10.0f, 0.0f, -114.0f},
                                                            (Vector3){0.0f, 0.0f, -114.0f}, (Vector3){30.0f, 0.0f, -114.0f},
                                                            (Vector3){40.0f, 0.0f, -114.0f}, (Vector3){50.0f, 0.0f, -114.0f},
                                                            (Vector3){-20.0f, 0.0f, -184.0f}, (Vector3){-10.0f, 0.0f, -184.0f},
                                                            (Vector3){30.0f, 0.0f, -184.0f}, (Vector3){40.0f, 0.0f, -184.0f},
                                                            (Vector3){50.0f, 0.0f, -184.0f}, (Vector3){-40.0f, 0.0f, -194.0f}, 
                                                            (Vector3){-30.0f, 0.0f, -194.0f}, (Vector3){-20.0f, 0.0f, -194.0f}, 
                                                            (Vector3){-10.0f, 0.0f, -194.0f}, (Vector3){0.0f, 0.0f, -194.0f},
                                                            (Vector3){30.0f, 0.0f, -194.0f}, (Vector3){40.0f, 0.0f, -194.0f},
                                                            (Vector3){50.0f, 0.0f, -194.0f}, (Vector3){60.0f, 0.0f, -194.0f},
                                                            (Vector3){70.0f, 0.0f, -194.0f}, (Vector3){-40.0f, 0.0f, -264.0f}, 
                                                            (Vector3){-30.0f, 0.0f, -264.0f}, (Vector3){-20.0f, 0.0f, -264.0f},
                                                            (Vector3){-10.0f, 0.0f, -264.0f}, (Vector3){0.0f, 0.0f, -264.0f},
                                                            (Vector3){10.0f, 0.0f, -264.0f}, (Vector3){20.0f, 0.0f, -264.0f},
                                                            (Vector3){30.0f, 0.0f, -264.0f}, (Vector3){40.0f, 0.0f, -264.0f},
                                                            (Vector3){50.0f, 0.0f, -264.0f}, (Vector3){60.0f, 0.0f, -264.0f}, 
                                                            (Vector3){70.0f, 0.0f, -264.0f}, (Vector3){0.0f, 0.0f, -184.0f} };
    
    //walls
    BoundingBox verticalWalls[VerticalWallCount];
    BoundingBox horizontalWalls[HorizontalWallCount];
    
    //bounding box for land battleship
    BoundingBox battleshipBox = GetMeshBoundingBox(BattleShipModel.meshes[0]);
    
    //setting position of vertical wall pieces
    for(int i = 0; i < VerticalWallCount; i++){
        verticalWalls[i] = GetMeshBoundingBox(Wall_Vertical.meshes[0]);
        verticalWalls[i].min = Vector3Add(verticalWalls[i].min, VerticalWallPositions[i]);
        verticalWalls[i].max = Vector3Add(verticalWalls[i].max, VerticalWallPositions[i]);
    }
    
    //setting position of horizontal wall pieces
    for(int i = 0; i < HorizontalWallCount; i++){
        horizontalWalls[i] = GetMeshBoundingBox(Wall_Horizontal.meshes[0]);
        horizontalWalls[i].min = Vector3Add(horizontalWalls[i].min, HorizontalWallPositions[i]);
        horizontalWalls[i].max = Vector3Add(horizontalWalls[i].max, HorizontalWallPositions[i]);
    }
    
    //buildings
    Vector3 Building1_Positions[] = {(Vector3){-13.0f, 0.0f, -1.0f}, (Vector3){15.0f, 0.0f, 0.0f},
                                     (Vector3){-15.0f, 0.0f, -12.0f}, (Vector3){0.0f, 0.0f, -12.0f},
                                     (Vector3){16.0f, 0.0f, -12.0f}, (Vector3){-15.0f, 0.0f, -24.0f},
                                     (Vector3){0.0f, 0.0f, -24.0f}, (Vector3){16.0f, 0.0f, -24.0f}, 
                                     (Vector3){-8.0f, 0.0f, -35.0f}, (Vector3){-8.0f, 0.0f, -36.0f},
                                     (Vector3){20.0f, 0.0f, -49.0f}, (Vector3){-13.0f, 0.0f, -69.0f}, 
                                     (Vector3){0.0f, 0.0f, -72.0f}, (Vector3){16.0f, 0.0f, -72.0f},
                                     (Vector3){16.0f, 0.0f, -84.0f}, (Vector3){-15.0f, 0.0f, -84.0f},
                                     (Vector3){-8.0f, 0.0f, -95.0f}, (Vector3){8.0f, 0.0f, -96.0f},
                                     (Vector3){-13.0f, 0.0f, -123.0f}, (Vector3){3.0f, 0.0f, -132.0f},
                                     (Vector3){25.0f, 0.0f, -132.0f}, (Vector3){45.0f, 0.0f, -130.0f},
                                     (Vector3){-11.0f, 0.0f, -144.0f}, (Vector3){-29.0f, 0.0f, -150.0f},
                                     (Vector3){45.0f, 0.0f, -150.0f}, (Vector3){-12.0f, 0.0f, -163.0f},
                                     (Vector3){0.0f, 0.0f, -173.0f}, (Vector3){18.0f, 0.0f, -166.0f},
                                     (Vector3){38.0f, 0.0f, -167.0f}, (Vector3){-13.0f, 0.0f, -1.0f},
                                     (Vector3){30.0f, 0.0f, -212.0f}, (Vector3){-13.0f, 0.0f, -212.0f},
                                     (Vector3){3.0f, 0.0f, -212.0f}, (Vector3){25.0f, 0.0f, -220.0f},
                                     (Vector3){45.0f, 0.0f, -210.0f}, (Vector3){64.0f, 0.0f, -210.0f},
                                     (Vector3){-30.0f, 0.0f, -226.0f}, (Vector3){-11.0f, 0.0f, -229.0f},
                                     (Vector3){11.0f, 0.0f, -229.0f}, (Vector3){29.0f, 0.0f, -230.0f},
                                     (Vector3){46.0f, 0.0f, -226.0f}, (Vector3){-63.0f, 0.0f, -226.0f},};
    //setting up bounding boxes for buildings
    BoundingBox building1_BBs[Building1Count];
    for(int i = 0; i < Building1Count; i++){
        building1_BBs[i] = GetMeshBoundingBox(Building1.meshes[0]);
        building1_BBs[i].min = Vector3Add(building1_BBs[i].min, Building1_Positions[i]);
        building1_BBs[i].max = Vector3Add(building1_BBs[i].max, Building1_Positions[i]);
    }
    
    typedef enum E_Type{
        TANK,
        APC
    } EnemyType;
    
    typedef enum P_Type{
        HEALTH,
        MAINGUN,
        MG
    } PickupType;
    
    //bullet data
    typedef struct BulletType{
        Model bulletModel;
        Vector3 bulletPos;
        Vector3 bulletDir;
        bool IsBulletFired;
        float bulletYaw;
        float bulletSpeed;
        float maxRange;
    } Bullet;
    
    Bullet playerTankBullets[MaxPlayerTankBullets]; //list of all main gun bullets the player can fire
    Bullet enemyTankBullets[MaxEnemyTankBullets]; //list of all enemy tank gun bullets
    Bullet playerMGBullets[MaxPlayerMGBullets];
    Bullet enemyMGBullets[MaxEnemyMGBullets];
    Bullet battleshipTankBullets[MaxNumberOfBattleShipTankBullets];
    Bullet battleshipSpecialBullets[MaxNumberOfSpecialBullets];
    
    //initializing all bullets
    for(int i = 0; i < MaxPlayerTankBullets; i++){
        playerTankBullets[i].bulletModel = tankBullet;
        playerTankBullets[i].bulletPos = (Vector3){0.0f, 0.0f, 0.0f};
        playerTankBullets[i].bulletDir = (Vector3){0.0f, 0.0f, 0.0f};
        playerTankBullets[i].IsBulletFired = false;
        playerTankBullets[i].bulletYaw = 0;
        playerTankBullets[i].bulletSpeed = 1;
        playerTankBullets[i].maxRange = 100;
    }
    
    for(int i = 0; i < MaxPlayerMGBullets; i++){
        playerMGBullets[i].bulletModel = MGBullet;
        playerMGBullets[i].bulletPos = (Vector3){0.0f, 0.0f, 0.0f};
        playerMGBullets[i].bulletDir = (Vector3){0.0f, 0.0f, 0.0f};
        playerMGBullets[i].IsBulletFired = false;
        playerMGBullets[i].bulletYaw = 0;
        playerMGBullets[i].bulletSpeed = 1;
        playerMGBullets[i].maxRange = 100;
    }
    
    //initializing enemy tank bullets
        for(int i = 0; i < MaxEnemyTankBullets; i++){
        enemyTankBullets[i].bulletModel = tankBullet;
        enemyTankBullets[i].bulletPos = (Vector3){0.0f, 0.0f, 0.0f};
        enemyTankBullets[i].bulletDir = (Vector3){0.0f, 0.0f, 0.0f};
        enemyTankBullets[i].IsBulletFired = false;
        enemyTankBullets[i].bulletYaw = 0;
        enemyTankBullets[i].bulletSpeed = 1;
        enemyTankBullets[i].maxRange = 20;
    }
    
    //initializing enemy mg bullets
    for(int i = 0; i < MaxEnemyMGBullets; i++){
        enemyMGBullets[i].bulletModel = MGBullet;
        enemyMGBullets[i].bulletPos = (Vector3){0.0f, 0.0f, 0.0f};
        enemyMGBullets[i].bulletDir = (Vector3){0.0f, 0.0f, 0.0f};
        enemyMGBullets[i].IsBulletFired = false;
        enemyMGBullets[i].bulletYaw = 0;
        enemyMGBullets[i].bulletSpeed = 1;
        enemyMGBullets[i].maxRange = 100;
    }
    
    //initializing battleship tank bullets
    for(int i = 0; i < MaxNumberOfBattleShipTankBullets; i++){
        battleshipSpecialBullets[i].bulletModel = BigBullet;
        battleshipSpecialBullets[i].bulletPos = (Vector3){0.0f, 0.0f, 0.0f};
        battleshipSpecialBullets[i].bulletDir = (Vector3){0.0f, 0.0f, 0.0f};
        battleshipSpecialBullets[i].IsBulletFired = false;
        battleshipSpecialBullets[i].bulletYaw = 0;
        battleshipSpecialBullets[i].bulletSpeed = 1;
        battleshipSpecialBullets[i].maxRange = 20;
    }
    
    //initializing battleship special bullets
    for(int i = 0; i < MaxNumberOfSpecialBullets; i++){
        battleshipTankBullets[i].bulletModel = tankBullet;
        battleshipTankBullets[i].bulletPos = (Vector3){0.0f, 0.0f, 0.0f};
        battleshipTankBullets[i].bulletDir = (Vector3){0.0f, 0.0f, 0.0f};
        battleshipTankBullets[i].IsBulletFired = false;
        battleshipTankBullets[i].bulletYaw = 0;
        battleshipTankBullets[i].bulletSpeed = 1;
        battleshipTankBullets[i].maxRange = 20;
    }
    
    //enemy data
    typedef struct enemyTank {
        EnemyType enemyType;
        Model enemyModel;
        Vector3 enemyPos;
        Vector3 enemyDir;
        float enemyYaw;
        float enemyRange;
        float enemyToPlayerAngle;
        float enemyTankFireRate;
        float enemyTimeTillLastShot;
        int enemyHealth;
        int enemyDamage;
        bool IsEnemyAlive;
        bool CanTankFire;
    }EnemyTank;
    
    EnemyTank enemyTanks[MaxNumberOfEnemyTanks]; //list of all enemy tanks
    Vector3 enemyTankPositions[] = {(Vector3){22.0f, 0.0f, -66.0f}, (Vector3){9.0f, 0.0f, -77.0f},
                                    (Vector3){-4.0f, 0.0f, -83.0f}, (Vector3){13.0f, 0.0f, -90.0f},
                                    (Vector3){-17.0f, 0.0f, -94.0f}, (Vector3){2.0f, 0.0f, -97.0f},
                                    (Vector3){-18.0f, 0.0f, -154.0f}, (Vector3){-1.0f, 0.0f, -154.0f},
                                    (Vector3){19.0f, 0.0f, -154.0f}, (Vector3){36.0f, 0.0f, -155.0f},
                                    (Vector3){9.0f, 0.0f, -174.0f}, (Vector3){29.0f, 0.0f, -175.0f},
                                    (Vector3){46.0f, 0.0f, -172.0f}, (Vector3){-36.0f, 0.0f, -231.0f},
                                    (Vector3){-22.0f, 0.0f, -228.0f}, (Vector3){-2.0f, 0.0f, -229.0f},
                                    (Vector3){19.0f, 0.0f, -230.0f}, (Vector3){40.0f, 0.0f, -230.0f},
                                    (Vector3){50.0f, 0.0f, -230.0f}, (Vector3){70.0f, 0.0f, -230.0f},};
    Vector3 enemyAPCPositions[] = {(Vector3){-9.0f, 0.0f, -30.0f}, (Vector3){8.0f, 0.0f, -30.0f},
                                   (Vector3){21.0f, 0.0f, -31.0f}, (Vector3){8.0f, 0.0f, -19.0f},
                                   (Vector3){-7.0f, 0.0f, -20.0f}, (Vector3){8.0f, 0.0f, -72.0f},
                                   (Vector3){-12.0f, 0.0f, -72.0f}, (Vector3){4.0f, 0.0f, -85.0f},
                                   (Vector3){35.0f, 0.0f, -221.0f}, (Vector3){53.0f, 0.0f, -221.0f},
                                   (Vector3){70.0f, 0.0f, -220.0f}, (Vector3){0.0f, 0.0f, -220.0f},
                                   (Vector3){-6.0f, 0.0f, -214.0f}, (Vector3){-21.0f, 0.0f, -214.0f},
                                   (Vector3){-35.0f, 0.0f, -210.0f},};
    EnemyTank enemyAPCs[MaxNumberOfEnemyAPCs];
    
    for(int i = 0; i < MaxNumberOfEnemyTanks; i++){
        enemyTanks[i].enemyType = TANK;
        enemyTanks[i].enemyModel = EnemyTankModel;
        enemyTanks[i].enemyPos = enemyTankPositions[i];
        enemyTanks[i].enemyDir = (Vector3){0.0f, 0.0f, 0.0f};
        enemyTanks[i].enemyHealth = 60;
        enemyTanks[i].enemyRange = 25;
        enemyTanks[i].enemyYaw = 180;
        enemyTanks[i].IsEnemyAlive = true;
        enemyTanks[i].enemyToPlayerAngle = 0;
        enemyTanks[i].enemyDamage = 10;
        enemyTanks[i].CanTankFire = true;
        enemyTanks[i].enemyTankFireRate = 2;
        enemyTanks[i].enemyTimeTillLastShot = 0;
    } //initializing list of enemy tanks
    
    //initializing list of enemy APCs
    for(int i = 0; i < MaxNumberOfEnemyAPCs; i++){
        enemyAPCs[i].enemyType = APC;
        enemyAPCs[i].enemyModel = EnemyAPCModel;
        enemyAPCs[i].enemyPos = enemyAPCPositions[i];
        enemyAPCs[i].enemyDir = (Vector3){0.0f, 0.0f, 0.0f};
        enemyAPCs[i].enemyHealth = 30;
        enemyAPCs[i].enemyRange = 15;
        enemyAPCs[i].enemyYaw = 180;
        enemyAPCs[i].IsEnemyAlive = true;
        enemyAPCs[i].enemyToPlayerAngle = 0;
        enemyAPCs[i].enemyDamage = 1;
        enemyAPCs[i].CanTankFire = true;
        enemyAPCs[i].enemyTankFireRate = 0.125f;
        enemyAPCs[i].enemyTimeTillLastShot = 0;
    }
    
    //pickups
    typedef struct pickup{
        PickupType pickupType;
        Vector3 pickupPos;
        float pickupYaw;
        Model pickupModel;
        bool IsPickedUp;
        float pickupRotSpeed;
    } Pickup;
    
    //data structure for holding type and position of pickups
    typedef struct p_Data{
        PickupType type;
        Vector3 pos;
    }PickupData;
    
    PickupData pickupData[] = {{HEALTH, (Vector3){-14.0f, 0.0f, -7.0f}}, {HEALTH, (Vector3){20.0f, 0.0f, -44.0f}},
                               {HEALTH, (Vector3){18.0f, 0.0f, -78.0f}}, {HEALTH, (Vector3){-12.0f, 0.0f, -56.0f}},
                               {HEALTH, (Vector3){-15.0f, 0.0f, -79.0f}}, {HEALTH, (Vector3){9.0f, 0.0f, -91.0f}},
                               {HEALTH, (Vector3){44.0f, 0.0f, -122.0f}}, {HEALTH, (Vector3){-13.0f, 0.0f, -188.0f}},
                               {HEALTH, (Vector3){-11.0f, 0.0f, -139.0f}}, {HEALTH, (Vector3){45.0f, 0.0f, -144.0f}},
                               {HEALTH, (Vector3){-11.0f, 0.0f, -178.0f}}, {HEALTH, (Vector3){37.0f, 0.0f, -181.0f}},
                               {HEALTH, (Vector3){66.0f, 0.0f, -200.0f}}, {HEALTH, (Vector3){-46.0f, 0.0f, -200.0f}},
                               {HEALTH, (Vector3){-12.0f, 0.0f, -201.0f}}, {HEALTH, (Vector3){-30.0f, 0.0f, -203.0f}},
                               {HEALTH, (Vector3){3.0f, 0.0f, -202.0f}}, {HEALTH, (Vector3){25.0f, 0.0f, -202.0f}},
                               {MG, (Vector3){15.0f, 0.0f, -6.0f}}, {MG, (Vector3){0.0f, 0.0f, -6.0f}},
                               {MG, (Vector3){16.0f, 0.0f, -18.0f}}, {MG, (Vector3){5.0f, 0.0f, -42.0f}},
                               {MG, (Vector3){0.0f, 0.0f, -67.0f}}, {MG, (Vector3){16.0f, 0.0f, -67.0f}},
                               {MG, (Vector3){16.0f, 0.0f, -67.0f}}, {MG, (Vector3){-7.0f, 0.0f, -90.0f}},
                               {MG, (Vector3){3.0f, 0.0f, -127.0f}}, {MG, (Vector3){25.0f, 0.0f, -125.0f}},
                               {MG, (Vector3){44.0f, 0.0f, -125.0f}}, {MG, (Vector3){29.0f, 0.0f, -145.0f}},
                               {MG, (Vector3){1.0f, 0.0f, -166.0f}}, {MG, (Vector3){38.0f, 0.0f, -162.0f}},
                               {MG, (Vector3){25.0f, 0.0f, -205.0f}}, {MG, (Vector3){2.0f, 0.0f, -207.0f}},
                               {MG, (Vector3){-14.0f, 0.0f, -207.0f}}, {MG, (Vector3){-30.0f, 0.0f, -207.0f}},
                               {MG, (Vector3){-30.0f, 0.0f, -220.0f}}, {MG, (Vector3){-11.0f, 0.0f, -224.0f}},
                               {MG, (Vector3){11.0f, 0.0f, -224.0f}}, {MG, (Vector3){29.0f, 0.0f, -225.0f}},
                               {MG, (Vector3){46.0f, 0.0f, -221.0f}}, {MG, (Vector3){63.0f, 0.0f, -221.0f}},
                               {MG, (Vector3){64.0f, 0.0f, -205.0f}}, {MG, (Vector3){45.0f, 0.0f, -204.0f}},
                               {MAINGUN, (Vector3){-11.0f, 0.0f, -134.0f}}, {MAINGUN, (Vector3){-30.0f, 0.0f, -217.0f}},
                               {MAINGUN, (Vector3){-11.0f, 0.0f, -217.0f}}, {MAINGUN, (Vector3){13.0f, 0.0f, -219.0f}},
                               {MAINGUN, (Vector3){-27.0f, 0.0f, -217.0f}}, {MAINGUN, (Vector3){-44.0f, 0.0f, -217.0f}},
                               {MAINGUN, (Vector3){29.0f, 0.0f, -219.0f}}, {MAINGUN, (Vector3){26.0f, 0.0f, -168.0f}},};
    
    Pickup AllPickups[MaxNumberOfPickups];
    
    //initializing pickups
    for(int i = 0; i < MaxNumberOfPickups; i++){
        AllPickups[i].pickupType = pickupData[i].type;
        AllPickups[i].pickupPos = pickupData[i].pos;
        AllPickups[i].pickupYaw = 0;
        if(AllPickups[i].pickupType == HEALTH) AllPickups[i].pickupModel = HealthPickup;
        else if(AllPickups[i].pickupType == MAINGUN) AllPickups[i].pickupModel = MainGunPickup;
        else AllPickups[i].pickupModel = MGPickup;
        AllPickups[i].IsPickedUp = false;
        AllPickups[i].pickupRotSpeed = 1;
    }
    
    //player attributes
    Vector3 playerPos = {0.0f, 0.0f, 0.0f};
    int PlayerDamage = 45;
    int PlayerMGDamage = 15;
    int PlayerHealth = 150;
    int CurrentPlayerHealth = 100;
    int CurrentMainGunAmmo = 20;
    int CurrentMGAmmo = 150;
    
    //positions for battleship guns
    Vector3 BattleshipTankGunPositions[] = {(Vector3){-45.9f, 0.0f, 12.0f}, (Vector3){-36.2f, 0.0f, 12.0f},
                                            (Vector3){-21.8f, 0.0f, 12.0f}, (Vector3){-14.8f, 0.0f, 12.0f},
                                            (Vector3){-5.1f, 0.0f, 12.0f}, (Vector3){10.6f, 0.0f, 12.0f},
                                            (Vector3){18.0f, 0.0f, 12.0f}, (Vector3){27.7f, 0.0f, 12.0f},
                                            (Vector3){46.1f, 0.0f, 12.0f}, (Vector3){55.8f, 0.0f, 12.0f}};
    Vector3 BattleshipSpecialGunPositions[] = {(Vector3){-40.3f, 0.0f, 15.0f}, (Vector3){-39.3f, 0.0f, 15.0f},
                                               (Vector3){-9.0f, 0.0f, 15.0f}, (Vector3){-8.0f, 0.0f, 15.0f},
                                               (Vector3){23.9f, 0.0f, 15.0f}, (Vector3){24.9f, 0.0f, 15.0f},
                                               (Vector3){52.1f, 0.0f, 15.0f}, (Vector3){53.1f, 0.0f, 15.0f},};
    
    //stuff for camera following player tank
    Vector3 camOffset = (Vector3){cam.position.x - playerPos.x, cam.position.y - playerPos.y, cam.position.z - playerPos.z};
    
    //stuff for controlling rate of fire of player's guns
    float playerYaw = 180;
    float playerTankGunTime = 0;
    float playerMGTime = 0;
    bool CanPlayerFireTank = true;
    bool CanPlayerFireMG = true;
    
    DisableCursor();
    
    //level related stuff
    Vector3 Level_Pos = (Vector3){-31.0f, 0.0f, 0.0f};
    Vector3 battleship_Pos = (Vector3){15.0f, 0.0f, -251.0f};
    LevelModel.transform = MatrixRotateY(DEG2RAD * 180);
    BattleShipModel.transform = MatrixRotateY(DEG2RAD * 180);
    battleshipBox.min = Vector3Add(battleshipBox.min, battleship_Pos);
    battleshipBox.max = Vector3Add(battleshipBox.max, battleship_Pos);
    int CurrentBattleshipHealth = MaxBattleshipHealth;
    bool CanBattleshipFire = true;
    float battleshipFireTime = 0.0f;
    
    //gameScreen related stuff
    bool ToRestartGame = false;
    bool IsPlayerDead = false;
    bool IsGameFinished = false;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float dt = GetFrameTime();
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&cam, CAMERA_FREE);
        
        // TODO: Update your variables here
        
        //detect input and move player character if player isn't dead
        if(!IsPlayerDead){
            if(IsKeyDown(KEY_RIGHT)) playerYaw -= playerMoveSpeed * 10;
            if(IsKeyDown(KEY_LEFT)) playerYaw += playerMoveSpeed * 10;
            if(IsKeyDown(KEY_UP)) {
                bool CanMove = true;
                Vector3 checkingSphereDist = (Vector3){playerPos.x + sin(DEG2RAD * playerYaw) * 1, 0.0f, playerPos.z + cos(DEG2RAD * playerYaw) * 1};
                for(int i = 0; i < HorizontalWallCount; i++){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], checkingSphereDist, 1)) CanMove = false;
                }
                for(int i = 0; i < VerticalWallCount; i++){
                    if(CheckCollisionBoxSphere(verticalWalls[i], checkingSphereDist, 1)) CanMove = false;
                }
                for(int i = 0; i < Building1Count; i++){
                    if(CheckCollisionBoxSphere(building1_BBs[i], checkingSphereDist, 1)) CanMove = false;
                }
                if(CheckCollisionBoxSphere(battleshipBox, checkingSphereDist, 1)) CanMove = false;
                if(CanMove){
                    playerPos.z += cos(DEG2RAD * playerYaw) * playerMoveSpeed;
                    playerPos.x += sin(DEG2RAD * playerYaw) * playerMoveSpeed; 
                }
            }
            if(IsKeyDown(KEY_DOWN)) {
                bool CanMove = true;
                Vector3 checkingSphereDist = (Vector3){playerPos.x - sin(DEG2RAD * playerYaw) * 2, 0.0f, playerPos.z - cos(DEG2RAD * playerYaw) * 2};
                for(int i = 0; i < HorizontalWallCount; i++){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], checkingSphereDist, 1)) CanMove = false;
                }
                for(int i = 0; i < VerticalWallCount; i++){
                    if(CheckCollisionBoxSphere(verticalWalls[i], checkingSphereDist, 1)) CanMove = false;
                }
                for(int i = 0; i < Building1Count; i++){
                    if(CheckCollisionBoxSphere(building1_BBs[i], checkingSphereDist, 1)) CanMove = false;
                }
                if(CanMove){
                    playerPos.z -= cos(DEG2RAD * playerYaw) * playerMoveSpeed;
                    playerPos.x -= sin(DEG2RAD * playerYaw) * playerMoveSpeed;  
                }  
            }
        }
        
        //updating player stuff
        cam.position = (Vector3) {camOffset.x + playerPos.x, camOffset.y + playerPos.y, camOffset.z + playerPos.z};
        cam.target = playerPos;
        playerTank.transform = MatrixRotateY(DEG2RAD * playerYaw);
        
        //updating rotation of all pickup items not picked up by player
        for(int i = 0; i < MaxNumberOfPickups; i++){
            if(!AllPickups[i].IsPickedUp){
                AllPickups[i].pickupYaw += AllPickups[i].pickupRotSpeed;
                AllPickups[i].pickupModel.transform = MatrixRotateY(DEG2RAD * AllPickups[i].pickupYaw);
            }
        }
        
        //checking enemy position and rotation and checking if enemy is dead and if player is in range
        for(int i = 0; i < MaxNumberOfEnemyTanks; i++){
            if(enemyTanks[i].IsEnemyAlive){
                if(Vector3Distance(enemyTanks[i].enemyPos, playerPos) <= enemyTanks[i].enemyRange){
                    enemyTanks[i].enemyDir = (Vector3){cos(DEG2RAD * enemyTanks[i].enemyYaw), 0.0f, sin(DEG2RAD * enemyTanks[i].enemyYaw)};
                    Vector3 enemyToPlayerDir = (Vector3){playerPos.x - enemyTanks[i].enemyPos.x,
                                                        playerPos.y - enemyTanks[i].enemyPos.y,
                                                        playerPos.z - enemyTanks[i].enemyPos.z};
                    float angleToRotate = -(atan2(enemyToPlayerDir.z,enemyToPlayerDir.x)) * 180.0f/3.14f + 90.0f;
                    enemyTanks[i].enemyToPlayerAngle = angleToRotate;
                    float angleError = angleToRotate - enemyTanks[i].enemyYaw;
                    
                    if(fabs(angleError) > 1){
                        if(angleError > 1) enemyTanks[i].enemyYaw += 1;
                        else if(angleError < 1) enemyTanks[i].enemyYaw -= 1;
                    }else{
                        //shoot at player tank
                        if(enemyTanks[i].CanTankFire){
                            for(int j = 0; j < MaxEnemyTankBullets; j++){
                                if(!enemyTankBullets[j].IsBulletFired){
                                    enemyTankBullets[i].bulletPos = (Vector3){enemyTanks[i].enemyPos.x,
                                                                              enemyTanks[i].enemyPos.y + 0.5f,
                                                                              enemyTanks[i].enemyPos.z};
                                    enemyTankBullets[j].bulletYaw = enemyTanks[i].enemyYaw;
                                    enemyTankBullets[j].bulletDir = enemyTanks[i].enemyDir;
                                    enemyTankBullets[j].IsBulletFired = true;
                                    enemyTanks[i].CanTankFire = false;
                                    PlaySound(EnemyTankGunSound);
                                }
                            }
                        }
                    }
                    
                    enemyTanks[i].enemyModel.transform = MatrixRotateY(DEG2RAD * enemyTanks[i].enemyYaw * 3.0f);
                }  
                if(enemyTanks[i].enemyHealth <= 0){
                    enemyTanks[i].IsEnemyAlive = false;
                }    
            }
        } 
        
        //checking enemy APC position and rotation and checking if enemy APC is dead and if player is in range
        for(int i = 0; i < MaxNumberOfEnemyAPCs; i++){
            if(enemyAPCs[i].IsEnemyAlive){
                if(Vector3Distance(enemyAPCs[i].enemyPos, playerPos) <= enemyAPCs[i].enemyRange){
                    enemyAPCs[i].enemyDir = (Vector3){cos(DEG2RAD * enemyAPCs[i].enemyYaw), 0.0f, sin(DEG2RAD * enemyAPCs[i].enemyYaw)};
                    Vector3 enemyToPlayerDir = (Vector3){playerPos.x - enemyAPCs[i].enemyPos.x,
                                                        playerPos.y - enemyAPCs[i].enemyPos.y,
                                                        playerPos.z - enemyAPCs[i].enemyPos.z};
                    float angleToRotate = -(atan2(enemyToPlayerDir.z,enemyToPlayerDir.x)) * 180.0f/3.14f + 90.0f;
                    enemyAPCs[i].enemyToPlayerAngle = angleToRotate;
                    float angleError = angleToRotate - enemyAPCs[i].enemyYaw;
                    
                    if(fabs(angleError) > 1){
                        if(angleError > 1) enemyAPCs[i].enemyYaw += 1;
                        else if(angleError < 1) enemyAPCs[i].enemyYaw -= 1;
                    }else{
                        //shoot machine gun at player tank 
                        if(enemyAPCs[i].CanTankFire){
                            for(int j = 0; j < MaxEnemyMGBullets; j++){
                                if(!enemyMGBullets[j].IsBulletFired){
                                    enemyMGBullets[i].bulletPos = (Vector3){enemyAPCs[i].enemyPos.x,
                                                                              enemyAPCs[i].enemyPos.y + 0.5f,
                                                                              enemyAPCs[i].enemyPos.z};
                                    enemyMGBullets[j].bulletYaw = enemyAPCs[i].enemyYaw;
                                    enemyMGBullets[j].bulletDir = enemyAPCs[i].enemyDir;
                                    enemyMGBullets[j].IsBulletFired = true;
                                    enemyAPCs[i].CanTankFire = false;
                                    PlaySound(PlayerMGSound);
                                }
                            }
                        }
                    }
                    
                    enemyAPCs[i].enemyModel.transform = MatrixRotateY(DEG2RAD * enemyAPCs[i].enemyYaw);
                }  
                if(enemyAPCs[i].enemyHealth <= 0){
                    enemyAPCs[i].IsEnemyAlive = false;
                }    
            }
        }
        
        //checking if battleship can fire
        if(CurrentBattleshipHealth > 0){
            //checking if player is close enough
            if(Vector3Distance(playerPos, battleship_Pos) <= 60){
                //display battleship health bar
                //fire battleship's guns
                if(CanBattleshipFire){
                    for(int i = 0; i < 10; i++){
                        for(int j = 0; j < MaxNumberOfBattleShipTankBullets; j++){
                            if(!battleshipTankBullets[j].IsBulletFired){
                                battleshipTankBullets[j].bulletPos = Vector3Add(BattleshipTankGunPositions[i], battleship_Pos);
                                battleshipTankBullets[j].bulletYaw = 0;
                                battleshipTankBullets[j].bulletDir = (Vector3){0.0f, 0.0f, 0.0f};
                                battleshipTankBullets[j].IsBulletFired = true;
                                break;
                            }
                        }
                    }
                    for(int i = 0; i < 8; i++){
                        for(int j = 0; j < MaxNumberOfSpecialBullets; j++){
                            if(!battleshipSpecialBullets[j].IsBulletFired){
                                battleshipSpecialBullets[j].bulletPos = Vector3Add(BattleshipSpecialGunPositions[i], battleship_Pos);
                                battleshipSpecialBullets[j].bulletYaw = 0;
                                battleshipSpecialBullets[j].bulletDir = (Vector3){0.0f, 0.0f, 0.0f};
                                battleshipSpecialBullets[j].IsBulletFired = true;
                                break;
                            }
                        }
                    }
                    
                    CanBattleshipFire = false;
                }
                
                if(!CanBattleshipFire){
                    battleshipFireTime += dt;
                    
                    if(battleshipFireTime >= BattleshipFireRate){
                        battleshipFireTime = 0;
                        CanBattleshipFire = true;
                    }
                }
            }
        }
        
        //updating player tank bullet
        for(int i = 0; i < MaxPlayerTankBullets; i++){
            if(playerTankBullets[i].IsBulletFired){
                playerTankBullets[i].bulletPos.z += cos(DEG2RAD * playerTankBullets[i].bulletYaw) * playerTankBullets[i].bulletSpeed;
                playerTankBullets[i].bulletPos.x += sin(DEG2RAD * playerTankBullets[i].bulletYaw) * playerTankBullets[i].bulletSpeed;
                playerTankBullets[i].bulletModel.transform = MatrixRotateY(DEG2RAD * playerTankBullets[i].bulletYaw);
                if(Vector3Distance(playerPos, playerTankBullets[i].bulletPos) >= playerTankBullets[i].maxRange){
                    playerTankBullets[i].IsBulletFired = false;
                }
            }
        }
        //updating player mg bullets
        for(int i = 0; i < MaxPlayerMGBullets; i++){
            if(playerMGBullets[i].IsBulletFired){
                playerMGBullets[i].bulletPos.z += cos(DEG2RAD * playerMGBullets[i].bulletYaw) * playerMGBullets[i].bulletSpeed;
                playerMGBullets[i].bulletPos.x += sin(DEG2RAD * playerMGBullets[i].bulletYaw) * playerMGBullets[i].bulletSpeed;
                playerMGBullets[i].bulletModel.transform = MatrixRotateY(DEG2RAD * playerMGBullets[i].bulletYaw);
                if(Vector3Distance(playerPos, playerMGBullets[i].bulletPos) >= playerMGBullets[i].maxRange){
                    playerMGBullets[i].IsBulletFired = false;
                }
            }
        }
        //updating enemy bullets
        for(int i = 0; i < MaxEnemyTankBullets; i++){
            if(enemyTankBullets[i].IsBulletFired){
                enemyTankBullets[i].bulletPos.z += cos(DEG2RAD * enemyTankBullets[i].bulletYaw) * enemyTankBullets[i].bulletSpeed;
                enemyTankBullets[i].bulletPos.x += sin(DEG2RAD * enemyTankBullets[i].bulletYaw) * enemyTankBullets[i].bulletSpeed;
                enemyTankBullets[i].bulletModel.transform = MatrixRotateY(DEG2RAD * enemyTankBullets[i].bulletYaw);
                if(Vector3Distance(playerPos, enemyTankBullets[i].bulletPos) >= enemyTankBullets[i].maxRange){
                    enemyTankBullets[i].IsBulletFired = false;
                }
            }
        }
        
        //updating enemy MG bullets
        for(int i = 0; i < MaxEnemyMGBullets; i++){
            if(enemyMGBullets[i].IsBulletFired){
                enemyMGBullets[i].bulletPos.z += cos(DEG2RAD * enemyMGBullets[i].bulletYaw) * enemyMGBullets[i].bulletSpeed;
                enemyMGBullets[i].bulletPos.x += sin(DEG2RAD * enemyMGBullets[i].bulletYaw) * enemyMGBullets[i].bulletSpeed;
                enemyMGBullets[i].bulletModel.transform = MatrixRotateY(DEG2RAD * enemyMGBullets[i].bulletYaw);
                if(Vector3Distance(playerPos, enemyMGBullets[i].bulletPos) >= enemyMGBullets[i].maxRange){
                    enemyMGBullets[i].IsBulletFired = false;
                }
            }
        }
        
        //updating battleship tank bullets
        for(int i = 0; i < MaxNumberOfBattleShipTankBullets; i++){
            if(battleshipTankBullets[i].IsBulletFired){
                battleshipTankBullets[i].bulletPos.z += cos(DEG2RAD * battleshipTankBullets[i].bulletYaw) * battleshipTankBullets[i].bulletSpeed;
                battleshipTankBullets[i].bulletPos.x += sin(DEG2RAD * battleshipTankBullets[i].bulletYaw) * battleshipTankBullets[i].bulletSpeed;
                battleshipTankBullets[i].bulletModel.transform = MatrixRotateY(DEG2RAD * battleshipTankBullets[i].bulletYaw);
                if(Vector3Distance(playerPos, battleshipTankBullets[i].bulletPos) >= battleshipTankBullets[i].maxRange){
                    battleshipTankBullets[i].IsBulletFired = false;
                }
            }
        }
        
        //updating battleship special bullets
        for(int i = 0; i < MaxNumberOfSpecialBullets; i++){
            if(battleshipSpecialBullets[i].IsBulletFired){
                battleshipSpecialBullets[i].bulletPos.z += cos(DEG2RAD * battleshipSpecialBullets[i].bulletYaw) * battleshipSpecialBullets[i].bulletSpeed;
                battleshipSpecialBullets[i].bulletPos.x += sin(DEG2RAD * battleshipSpecialBullets[i].bulletYaw) * battleshipSpecialBullets[i].bulletSpeed;
                battleshipSpecialBullets[i].bulletModel.transform = MatrixRotateY(DEG2RAD * battleshipSpecialBullets[i].bulletYaw);
                if(Vector3Distance(playerPos, battleshipSpecialBullets[i].bulletPos) >= battleshipSpecialBullets[i].maxRange){
                    battleshipSpecialBullets[i].IsBulletFired = false;
                }
            }
        }
        
        //checking if player bullets hit enemy
        for(int i = 0; i < MaxNumberOfEnemyTanks; i++){
            if(enemyTanks[i].IsEnemyAlive){
                for(int j = 0; j < MaxPlayerTankBullets; j++){
                    if(playerTankBullets[j].IsBulletFired){
                        if(CheckCollisionSpheres(playerTankBullets[j].bulletPos, 1, enemyTanks[i].enemyPos, 3)){
                            PlaySound(EnemyHitSound);
                            enemyTanks[i].enemyHealth -= PlayerDamage;
                            playerTankBullets[j].IsBulletFired = false;
                            
                            if(enemyTanks[i].enemyHealth <= 0){
                                enemyTanks[i].IsEnemyAlive = false;
                                PlaySound(EnemyDieSound);
                            }
                        }
                    }
                }//checking if player MG hits enemy tank
                for(int j = 0; j < MaxPlayerMGBullets; j++){
                    if(playerMGBullets[j].IsBulletFired){
                        if(CheckCollisionSpheres(playerMGBullets[j].bulletPos, 1, enemyTanks[i].enemyPos, 3)){
                            PlaySound(EnemyHitSound);
                            enemyTanks[i].enemyHealth -= PlayerMGDamage;
                            playerMGBullets[j].IsBulletFired = false;
                            
                            if(enemyTanks[i].enemyHealth <= 0){
                                enemyTanks[i].IsEnemyAlive = false;
                                PlaySound(EnemyDieSound);
                            }
                        }
                    }
                }
            }
        }
        
        //checking if player hits the battleship
        for(int i = 0; i < MaxPlayerTankBullets; i++){
            if(playerTankBullets[i].IsBulletFired){
                if(CheckCollisionBoxSphere(battleshipBox, playerTankBullets[i].bulletPos,1)){
                    PlaySound(EnemyHitSound);
                    CurrentBattleshipHealth -= PlayerDamage;
                    playerTankBullets[i].IsBulletFired = false;
                    break;
                }
            }
        }
        
        //checking if player mg hits the battleship
        for(int i = 0; i < MaxPlayerMGBullets; i++){
            if(playerMGBullets[i].IsBulletFired){
                    if(CheckCollisionBoxSphere(battleshipBox, playerMGBullets[i].bulletPos,1)){
                        PlaySound(EnemyHitSound);
                        CurrentBattleshipHealth -= PlayerMGDamage;
                        playerMGBullets[i].IsBulletFired = false;
                        break;
                }
            }
        }
        
        //checking if player bullets hit enemy APC
        for(int i = 0; i < MaxNumberOfEnemyAPCs; i++){
            if(enemyAPCs[i].IsEnemyAlive){
                for(int j = 0; j < MaxPlayerTankBullets; j++){
                    if(playerTankBullets[j].IsBulletFired){
                        if(CheckCollisionSpheres(playerTankBullets[j].bulletPos, 1, enemyAPCs[i].enemyPos, 3)){
                            PlaySound(EnemyHitSound);
                            enemyAPCs[i].enemyHealth -= PlayerDamage;
                            playerTankBullets[j].IsBulletFired = false;
                            
                            if(enemyAPCs[i].enemyHealth <= 0){
                                enemyAPCs[i].IsEnemyAlive = false;
                                PlaySound(EnemyDieSound);
                            }
                        }
                    }
                }//checking if player MG hits enemy APC
                for(int j = 0; j < MaxPlayerMGBullets; j++){
                    if(playerMGBullets[j].IsBulletFired){
                        if(CheckCollisionSpheres(playerMGBullets[j].bulletPos, 1, enemyAPCs[i].enemyPos, 3)){
                            PlaySound(EnemyHitSound);
                            enemyAPCs[i].enemyHealth -= PlayerMGDamage;
                            playerMGBullets[j].IsBulletFired = false;
                            
                            if(enemyAPCs[i].enemyHealth <= 0){
                                enemyAPCs[i].IsEnemyAlive = false;
                                PlaySound(EnemyDieSound);
                            }
                        }
                    }
                }
            }
        }
        
        //check if battleship bullets hit the player
        for(int i = 0; i < MaxNumberOfBattleShipTankBullets; i++){
            if(battleshipTankBullets[i].IsBulletFired){
                if(CheckCollisionSpheres(playerPos, 2, battleshipTankBullets[i].bulletPos, 1)){
                    CurrentPlayerHealth -= enemyTanks[0].enemyDamage;
                    battleshipTankBullets[i].IsBulletFired = false;
                }
            }
        }
        
        //check if battleship special bullets hit the player
        for(int i = 0; i < MaxNumberOfSpecialBullets; i++){
            if(battleshipSpecialBullets[i].IsBulletFired){
                if(CheckCollisionSpheres(playerPos, 2, battleshipSpecialBullets[i].bulletPos, 1)){
                    CurrentPlayerHealth -= SpecialBulletDamage;
                    battleshipSpecialBullets[i].IsBulletFired = false;
                }
            }
        }
        
        //check if player and enemy bullet hits walls
        for(int i = 0; i < HorizontalWallCount; i++){
            //checking for horizontal walls
            for(int j = 0; j < MaxPlayerTankBullets; j++){
                if(playerTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], playerTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        playerTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            for(int j = 0; j < MaxPlayerMGBullets; j++){
                if(playerMGBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], playerMGBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        playerMGBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxEnemyTankBullets; j++){
                if(enemyTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], enemyTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        enemyTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxEnemyMGBullets; j++){
                if(enemyMGBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], enemyMGBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        enemyMGBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            //checking for battleship bullets hitting walls
            for(int j = 0; j < MaxNumberOfBattleShipTankBullets; j++){
                if(battleshipTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], battleshipTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        battleshipTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxNumberOfSpecialBullets; j++){
                if(battleshipSpecialBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], battleshipSpecialBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        battleshipSpecialBullets[j].IsBulletFired = false;
                    }
                }
            }
        }
        
        //checking if player and enemy bullets hit vertical walls
        for(int i = 0; i < VerticalWallCount; i++){
            //checking for horizontal walls
            for(int j = 0; j < MaxPlayerTankBullets; j++){
                if(playerTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(verticalWalls[i], playerTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        playerTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            for(int j = 0; j < MaxPlayerMGBullets; j++){
                if(playerMGBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(verticalWalls[i], playerMGBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        playerMGBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxEnemyTankBullets; j++){
                if(enemyTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(verticalWalls[i], enemyTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        enemyTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxEnemyMGBullets; j++){
                if(enemyMGBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(verticalWalls[i], enemyMGBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        enemyMGBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            //checking for battleship bullets hitting walls
            for(int j = 0; j < MaxNumberOfBattleShipTankBullets; j++){
                if(battleshipTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], battleshipTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        battleshipTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxNumberOfSpecialBullets; j++){
                if(battleshipSpecialBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], battleshipSpecialBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        battleshipSpecialBullets[j].IsBulletFired = false;
                    }
                }
            }
        }
        
        //checking if player and enemy bullets hit buildings
        for(int i = 0; i < Building1Count; i++){
            //checking for horizontal walls
            for(int j = 0; j < MaxPlayerTankBullets; j++){
                if(playerTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(building1_BBs[i], playerTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        playerTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            for(int j = 0; j < MaxPlayerMGBullets; j++){
                if(playerMGBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(building1_BBs[i], playerMGBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        playerMGBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxEnemyTankBullets; j++){
                if(enemyTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(building1_BBs[i], enemyTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        enemyTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxEnemyMGBullets; j++){
                if(enemyMGBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(building1_BBs[i], enemyMGBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        enemyMGBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            //checking for battleship bullets hitting buildings
            for(int j = 0; j < MaxNumberOfBattleShipTankBullets; j++){
                if(battleshipTankBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], battleshipTankBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        battleshipTankBullets[j].IsBulletFired = false;
                    }
                }
            }
            
            for(int j = 0; j < MaxNumberOfSpecialBullets; j++){
                if(battleshipSpecialBullets[j].IsBulletFired){
                    if(CheckCollisionBoxSphere(horizontalWalls[i], battleshipSpecialBullets[j].bulletPos, 1)){
                        //play wall hit sound
                        battleshipSpecialBullets[j].IsBulletFired = false;
                    }
                }
            }
        }
        
        //checking if enemy bullets hit the player
        for(int i = 0; i < MaxEnemyTankBullets; i++){
            if(enemyTankBullets[i].IsBulletFired){
                if(CheckCollisionSpheres(enemyTankBullets[i].bulletPos, 1, playerPos, 2)){
                    CurrentPlayerHealth -= enemyTanks[0].enemyDamage;
                    enemyTankBullets[i].IsBulletFired = false;
                }
            }
        }
        
        //checking if enemy MG bullets hit the player
        for(int i = 0; i < MaxEnemyMGBullets; i++){
            if(enemyMGBullets[i].IsBulletFired){
                if(CheckCollisionSpheres(enemyMGBullets[i].bulletPos, 1, playerPos, 2)){
                    CurrentPlayerHealth -= enemyAPCs[0].enemyDamage;
                    enemyMGBullets[i].IsBulletFired = false;
                }
            }
        }
        
        //checking if player fires bullet
        if(CanPlayerFireTank && !IsPlayerDead){
            if(IsKeyPressed(KEY_SPACE) && CurrentMainGunAmmo > 0){
                for(int i = 0; i < MaxPlayerTankBullets; i++){
                    if(!playerTankBullets[i].IsBulletFired){
                        playerTankBullets[i].bulletPos = (Vector3){playerPos.x, playerPos.y + 0.6f, playerPos.z + 0.2f};
                        playerTankBullets[i].bulletYaw = playerYaw;
                        playerTankBullets[i].bulletDir = (Vector3){sin(DEG2RAD * playerYaw), 0, cos(DEG2RAD * playerYaw)};
                        playerTankBullets[i].IsBulletFired = true;
                        CurrentMainGunAmmo--;
                        break;
                    }
                }
                PlaySound(PlayerTankGunSound);
                CanPlayerFireTank = false;
            }
        }
        
        //check if player fires machine gun
        if(CanPlayerFireMG){
            if(IsKeyDown(KEY_RIGHT_ALT) && CurrentMGAmmo > 0){
                for(int i = 0; i < MaxPlayerMGBullets; i++){
                    if(!playerMGBullets[i].IsBulletFired){
                        playerMGBullets[i].bulletPos = Vector3Add(playerPos, (Vector3){-0.1f, 0.9f, 0.1f});
                        playerMGBullets[i].bulletYaw = playerYaw;
                        playerMGBullets[i].bulletDir = (Vector3){sin(DEG2RAD * playerYaw), 0, cos(DEG2RAD * playerYaw)};
                        playerMGBullets[i].IsBulletFired = true;
                        CurrentMGAmmo--;
                        break;
                    }
                }
                PlaySound(PlayerMGSound);
                CanPlayerFireMG = false;
            }
        }
        
        //check if player has picked up any pickup
        for(int i = 0; i < MaxNumberOfPickups; i++){
            if(!AllPickups[i].IsPickedUp){
                if(CheckCollisionSpheres(AllPickups[i].pickupPos,1, playerPos, 3)){
                    if(AllPickups[i].pickupType == HEALTH){
                        if(CurrentPlayerHealth < PlayerHealth){
                            int healthDif = PlayerHealth - CurrentPlayerHealth;
                            if(healthDif >= HealthBoost) CurrentPlayerHealth += HealthBoost;
                            else CurrentPlayerHealth = PlayerHealth;
                            AllPickups[i].IsPickedUp = true;
                            PlaySound(HealthPickupSound);
                        }
                    }
                    if(AllPickups[i].pickupType == MAINGUN){
                        if(CurrentMainGunAmmo < MaxPlayerMainGunAmmo){
                            int ammoDif = MaxPlayerMainGunAmmo - CurrentMainGunAmmo;
                            if(ammoDif >= MainGunAmmoBoost) CurrentMainGunAmmo += MainGunAmmoBoost;
                            else CurrentMainGunAmmo = MaxPlayerMainGunAmmo;
                            AllPickups[i].IsPickedUp = true;
                            PlaySound(HealthPickupSound);
                        }
                    }
                    if(AllPickups[i].pickupType == MG){
                        if(CurrentMGAmmo < MaxPlayerMGAmmo){
                            int ammoDif = MaxPlayerMGAmmo - CurrentMGAmmo;
                            if(ammoDif >= MGAmmoBoost) CurrentMGAmmo += MGAmmoBoost;
                            else CurrentMGAmmo = MaxPlayerMGAmmo;
                            AllPickups[i].IsPickedUp = true;
                            PlaySound(HealthPickupSound);
                        }
                    }    
                }
            }
        }
        
        //checking if player can fire main gun again
        if(!CanPlayerFireTank) {
            playerTankGunTime += dt;
            if(playerTankGunTime > PlayerTankDelay) {
                playerTankGunTime = 0;
                CanPlayerFireTank = true;
            }
        }
        
        //check if player can fire machine gun again
        if(!CanPlayerFireMG){
            playerMGTime += dt;
            if(playerMGTime > PlayerMGDelay) {
                playerMGTime = 0;
                CanPlayerFireMG = true;
            }
        }
        
        //checking if enemy tanks can fire bullet
        for(int i = 0; i < MaxNumberOfEnemyTanks; i++){
            if(!enemyTanks[i].CanTankFire){
                enemyTanks[i].enemyTimeTillLastShot += dt;
                
                if(enemyTanks[i].enemyTimeTillLastShot >= enemyTanks[i].enemyTankFireRate){
                    enemyTanks[i].CanTankFire = true;
                    enemyTanks[i].enemyTimeTillLastShot = 0;
                }
            }
        }
        
        //checking if enemy APCs can fire bullet
        for(int i = 0; i < MaxNumberOfEnemyAPCs; i++){
            if(!enemyAPCs[i].CanTankFire){
                enemyAPCs[i].enemyTimeTillLastShot += dt;
                
                if(enemyAPCs[i].enemyTimeTillLastShot >= enemyAPCs[i].enemyTankFireRate){
                    enemyAPCs[i].CanTankFire = true;
                    enemyAPCs[i].enemyTimeTillLastShot = 0;
                }
            }
        }
        
        //check if player is dead
        if(CurrentPlayerHealth <= 0){
            CurrentPlayerHealth = 0;
            IsPlayerDead = true;
        }
        
        //check if game finished
        bool AreAllEnemiesDead = true;
        for(int i = 0; i < MaxNumberOfEnemyTanks; i++){
            if(enemyTanks[i].IsEnemyAlive) AreAllEnemiesDead = false;
        }
        for(int i = 0; i < MaxNumberOfEnemyAPCs; i++){
            if(enemyAPCs[i].IsEnemyAlive) AreAllEnemiesDead = false;
        }
        if(CurrentBattleshipHealth > 0) AreAllEnemiesDead = false;
        
        if(AreAllEnemiesDead) IsGameFinished = true;
        
        //check if player wants to restart game, and if yes, then restart game
        if(IsKeyPressed(KEY_R)){
            if(!ToRestartGame)ToRestartGame = true;
        }
        if(ToRestartGame){
            //reset pickups
            for(int i = 0; i < MaxNumberOfPickups; i++){
                AllPickups[i].IsPickedUp = false;
            }
            
            //resetting enemy tanks
            for(int i = 0; i < MaxNumberOfEnemyTanks; i++){
                enemyTanks[i].enemyHealth = 60;
                enemyTanks[i].IsEnemyAlive = true;
                enemyTanks[i].enemyYaw = 180;
            }
            
            //resetting enemy APCs
            for(int i = 0; i < MaxNumberOfEnemyAPCs; i++){
                enemyAPCs[i].enemyHealth = 45;
                enemyAPCs[i].IsEnemyAlive = true;
                enemyAPCs[i].enemyYaw = 180;
            }
            
            //resetting enemy boss
            int CurrentBattleshipHealth = MaxBattleshipHealth;
            bool CanBattleshipFire = true;
            float battleshipFireTime = 0.0f;
            
            //resetting player
            playerPos = (Vector3){0.0f, 0.0f, 0.0f};
            CurrentPlayerHealth = PlayerHealth;
            CurrentMainGunAmmo = MaxPlayerMainGunAmmo;
            CurrentMGAmmo = MaxPlayerMGAmmo;
            playerYaw = 180;
            CanPlayerFireMG = true;
            CanPlayerFireTank = true;
            playerTankGunTime = 0;
            playerMGTime = 0;
            
            IsPlayerDead = false;
            ToRestartGame = false;
            IsGameFinished = false;
            
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKGRAY);

        BeginMode3D(cam);
        //DrawGrid(32, 1);
        
        //drawing player tank
        DrawModel(playerTank, playerPos, 1.0f, WHITE);
        
        //drawing player all gun bullets
        for(int i = 0; i < MaxPlayerTankBullets; i++){
            if(playerTankBullets[i].IsBulletFired) DrawModel(playerTankBullets[i].bulletModel, playerTankBullets[i].bulletPos, 1.0f, WHITE);
        }
        for(int i = 0; i < MaxPlayerMGBullets; i++){
            if(playerMGBullets[i].IsBulletFired) DrawModel(playerMGBullets[i].bulletModel, playerMGBullets[i].bulletPos, 1.0f, WHITE);
        }
        
        //drawing enemy tank bullets
        for(int i = 0; i < MaxEnemyTankBullets; i++){
            if(enemyTankBullets[i].IsBulletFired) DrawModel(enemyTankBullets[i].bulletModel, enemyTankBullets[i].bulletPos, 1.0f, WHITE);
        }
        
        //drawing enemy machine gun bullets
        for(int i = 0; i < MaxEnemyMGBullets; i++){
            if(enemyMGBullets[i].IsBulletFired) DrawModel(enemyMGBullets[i].bulletModel, enemyMGBullets[i].bulletPos, 1.0f, WHITE);
        }
        
        //drawing battleship tank bullets
        for(int i = 0; i < MaxNumberOfBattleShipTankBullets; i++){
            if(battleshipTankBullets[i].IsBulletFired) DrawModel(battleshipTankBullets[i].bulletModel, battleshipTankBullets[i].bulletPos, 1.0f, WHITE);
        }
        
        //drawing battleship special bullets
        for(int i = 0; i < MaxNumberOfSpecialBullets; i++){
            if(battleshipSpecialBullets[i].IsBulletFired) DrawModel(battleshipSpecialBullets[i].bulletModel, battleshipSpecialBullets[i].bulletPos, 1.0f, WHITE);
        }
        
        //drawing enemy tanks
        for(int i = 0; i < MaxNumberOfEnemyTanks; i++){
            if(enemyTanks[i].IsEnemyAlive) if(Vector3Distance(playerPos, enemyTanks[i].enemyPos) <= 50)DrawModel(enemyTanks[i].enemyModel, enemyTanks[i].enemyPos, 1, WHITE);
        }
        
        //drawing enemy APCs
        for(int i = 0; i < MaxNumberOfEnemyAPCs; i++){
            if(enemyAPCs[i].IsEnemyAlive) if(Vector3Distance(playerPos, enemyAPCs[i].enemyPos) <= 50) DrawModel(enemyAPCs[i].enemyModel, enemyAPCs[i].enemyPos, 1, WHITE);
        }
        
        //drawing pickups
        for(int i =0; i < MaxNumberOfPickups; i++){
            if(!AllPickups[i].IsPickedUp) {
                if(Vector3Distance(playerPos, AllPickups[i].pickupPos) <= 50)DrawModel(AllPickups[i].pickupModel, AllPickups[i].pickupPos, 2, WHITE);
                Vector3 spherePos = Vector3Add(AllPickups[i].pickupPos, (Vector3){0.0f, 0.5f, 0.0f});
                if(AllPickups[i].pickupType == HEALTH) DrawSphere(spherePos, 1.0f, (Color){255, 0, 0, 50});
                else if(AllPickups[i].pickupType == MG) DrawSphere(spherePos, 1.0f, (Color){255, 203, 0, 50});
                else DrawSphere(spherePos, 1.0f, (Color){255, 255, 255, 50});
            }
        }
        
        /*//boundary test
        for(int i = 0; i < VerticalWallCount; i++){
            DrawBoundingBox(verticalWalls[i], DARKGRAY);
        }
        for(int i = 0; i < HorizontalWallCount; i++){
            DrawBoundingBox(horizontalWalls[i], DARKGRAY);
        }
        for(int i = 0; i < Building1Count; i++){
            DrawBoundingBox(building1_BBs[i], DARKGRAY);
        }*/
        
        //drawing bulding 1
        for(int i = 0; i < Building1Count; i++){
            if(Vector3Distance(playerPos, Building1_Positions[i]) <= 50) DrawModel(Building1, Building1_Positions[i], 1, WHITE);
        }
        
        //drawing level
        DrawModel(LevelModel, Level_Pos, 1.0f, WHITE);
        DrawModel(BattleShipModel, battleship_Pos, 1, WHITE);
        
        EndMode3D();
        DrawTextureEx(healthIcon_tex, (Vector2){25, GetScreenHeight()-100}, 0, 0.1375f, WHITE);
        DrawTextureEx(MainGunIcon_tex, (Vector2){25, GetScreenHeight()-230}, 0, 0.1375f, WHITE);
        DrawTextureEx(MGIcon_tex, (Vector2){25, GetScreenHeight()-360}, 0, 0.1375f, WHITE);
        DrawText(TextFormat("%d", CurrentPlayerHealth), 200, GetScreenHeight() - 100, 100, RAYWHITE);
        DrawText(TextFormat("%d", CurrentMainGunAmmo), 200, GetScreenHeight() - 230, 100, RAYWHITE);
        DrawText(TextFormat("%d", CurrentMGAmmo), 200, GetScreenHeight() - 360, 100, RAYWHITE);
        
        if(IsPlayerDead){
            DrawText("Game Over!", (int)GetScreenWidth()/2 - 400, (int)GetScreenHeight()/2 - 300, 200, RAYWHITE);
            DrawText("Press R to restart game", (int)GetScreenWidth()/2 - 300, (int)GetScreenHeight()/2, 50, RAYWHITE);
            DrawText("Press ESC to quit game", (int)GetScreenWidth()/2 - 300, (int)GetScreenHeight()/2 + 100, 50, RAYWHITE);
        }
        if(!IsPlayerDead && !IsGameFinished){
            DrawText("Destroy All Enemies", (int)GetScreenWidth()/2 - 300, 100, 50, RAYWHITE);
            
            if(Vector3Distance(playerPos, battleship_Pos) <= 100){
                DrawText("STRANDED LAND BATTLESHIP",(int)GetScreenWidth()/2 - 400, 200, 50, WHITE);
                DrawRectangle((int)GetScreenWidth()/2 - 400, 275, 800, 50, WHITE);
                DrawRectangle((int)GetScreenWidth()/2 - 400, 275, (CurrentBattleshipHealth/MaxBattleshipHealth * 800), 50, RED);
            }
            
            DrawText("Controls:", 25, 10, 20, RAYWHITE);
            DrawText("Arrow Up/Arrow Down - Move Forward/Backward", 25, 40, 20, RAYWHITE);
            DrawText("Arrow Right/Arrow Left - Turn Right/Left", 25, 60, 20, RAYWHITE);
            DrawText("Spacebar - Fire Tank Gun", 25, 80, 20, RAYWHITE);
            DrawText("Left Alt - Fire Machine Gun", 25, 100, 20, RAYWHITE);
        }
        
        //draw game finish screen
        if(IsGameFinished){
            DrawText("You won!", (int)GetScreenWidth()/2 - 400, (int)GetScreenHeight()/2 - 300, 200, RAYWHITE);
            DrawText("Press R to restart game", (int)GetScreenWidth()/2 - 300, (int)GetScreenHeight()/2, 50, RAYWHITE);
            DrawText("Press ESC to quit game", (int)GetScreenWidth()/2 - 300, (int)GetScreenHeight()/2 + 100, 50, RAYWHITE);
        }
        
        DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(playerTank);
    UnloadModel(EnemyTankModel);
    UnloadModel(tankBullet);
    UnloadModel(MGBullet);
    UnloadModel(HealthPickup);
    UnloadModel(MainGunPickup);
    UnloadModel(MGPickup);
    UnloadModel(EnemyAPCModel);
    UnloadModel(Wall_Vertical);
    UnloadModel(Wall_Horizontal);
    UnloadModel(Building1);
    UnloadModel(LevelModel);
    UnloadModel(BattleShipModel);
    UnloadModel(BigBullet);
    
    UnloadTexture(enemyAPC_tex);
    UnloadTexture(pickupTexture);
    UnloadTexture(enemyTank_tex);
    UnloadTexture(bulletTexture);
    UnloadTexture(playerTank_tex);
    UnloadTexture(building1_tex);
    UnloadTexture(level_tex);
    UnloadTexture(battleship_tex);
    UnloadTexture(healthIcon_tex);
    UnloadTexture(MainGunIcon_tex);
    UnloadTexture(MGIcon_tex);
    UnloadTexture(explosionFlipBookTexture);
    
    UnloadImage(GameIcon);
    
    UnloadSound(PlayerMGSound);
    UnloadSound(EnemyHitSound);
    UnloadSound(PlayerTankGunSound);
    UnloadSound(EnemyDieSound);
    UnloadSound(EnemyTankGunSound);
    UnloadSound(HealthPickupSound);
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}