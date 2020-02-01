#include<stdio.h>
#include<math.h>
#include<Windows.h>

#include "./cnsglib/include/cnsg.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define FRAME_PER_SECOND 60

#define STAGE_COLLISIONMASK 0x01
#define ENEMY_COLLISIONMASK 0x02

static FontSJIS shnm12;
static Scene gameScene;
static Image saboImage, saboCollisionImage;
static Node heroNode;
static Node gameoverNode;
static Node stageNode;
static Node scoreNode;
static int isGameover;
static int score;
static IntervalEventScene *scoreInterval;
// static float move[2];
// static ControllerData *moveData[4];

void spawnSabo(Scene *scene, float x, float y) {
  Node *node;
  node = malloc(sizeof(Node));
  *node = initNodeSprite("sabo", 50.0F, 50.0F, saboImage, saboCollisionImage);
  node->position[0] = x;
  node->position[1] = y;
  node->velocity[0] = -200.0F + -20.0F * floorf(score / 10.0F);
  node->collisionMaskActive = ENEMY_COLLISIONMASK;
  node->isThrough = TRUE;
  push(&scene->nodes, node);
}

int heroBehaviour(Node *node, float elapsed) {
  // node->velocity[1] = max(node->velocity[1], -20.0F);
  // if(move[1] > 0.0F && node->velocity[1] < 0.0F) node->velocity[1] = -10.0F;
  // if(move[1] < 0.0F) node->velocity[1] = -100.0F;
  // node->velocity[0] = 40.0F * move[0];
  if(node->collisionFlags & ENEMY_COLLISIONMASK) {
    isGameover = TRUE;
    gameoverNode.isVisible = TRUE;
    gameScene.speed = 0.0F;
  }
  return TRUE;
}

int gameSceneInterval(Scene *scene) {
  if(rand() % 3) spawnSabo(&gameScene, 200.0F, -25.0F);
  score += 1;
  return TRUE;
}

void startGame(void) {
  Node *node;
  srand(0);
  score = 0;
  isGameover = FALSE;
  gameoverNode.isVisible = FALSE;
  gameScene.speed = 1.0F;
  scoreInterval->counter = 0.0F;
  heroNode.position[1] = 0.0F;
  clearVec3(heroNode.velocity);
  iterf(&gameScene.nodes, &node) {
    if(node->collisionMaskActive & ENEMY_COLLISIONMASK) {
      removeByData(&gameScene.nodes, node);
      discardNode(node);
      free(node);
    }
  }

}

int scoreBehaviour(Node *node, float elapsed) {
  char buffer[11];
  sprintf(buffer, "SCORE %04d", score);
  drawTextSJIS(node->texture, shnm12, 0.0F, 0.0F, buffer);
  return TRUE;
}

void spaceEvent(void) {
  if(heroNode.collisionFlags & STAGE_COLLISIONMASK) heroNode.impulseForce[1] += 400.0F;
}

void initGame(void) {
  shnm12 = initFontSJIS(loadBitmap("assets/shnm6x12r.bmp", NULL_COLOR), loadBitmap("assets/shnmk12.bmp", NULL_COLOR), 6, 12, 12);
  initControllerEvent('W', spaceEvent, NULL);
  initControllerEvent('R', NULL, startGame);
  // initControllerDataCross(moveData, 'W', 'A', 'S', 'D', move);
  gameScene = initScene();
  setCurrentScene(&gameScene, NULL, 0.0F);
  gameScene.background = BLUE;
  gameScene.acceleration[1] = -1000.0F;
  gameScene.camera.position[2] = -100.0F;
  scoreInterval = addIntervalEventScene(&gameScene, 1.0F, gameSceneInterval);
  saboImage = loadBitmap("./assets/sabo.bmp", WHITE);
  saboCollisionImage = loadBitmap("./assets/saboCollision.bmp", NULL_COLOR);
  heroNode = initNodeSprite("hero", 75.0F, 75.0F, loadBitmap("./assets/hero.bmp", WHITE), loadBitmap("./assets/heroCollision.bmp", NULL_COLOR));
  heroNode.position[0] = -100.0F;
  heroNode.isGravityEnabled = TRUE;
  heroNode.collisionMaskPassive = STAGE_COLLISIONMASK | ENEMY_COLLISIONMASK;
  heroNode.behaviour = heroBehaviour;
  push(&gameScene.nodes, &heroNode);
  gameoverNode = initNodeText("gameover", 0.0F, 0.0F, CENTER, CENTER, 72, 36, NULL);
  drawTextSJIS(gameoverNode.texture, shnm12, 0.0F, 0.0F, "GAMEOVER\nPRESS \"R\"\nTO CONTINUE.");
  push(&gameScene.nodes, &gameoverNode);
  scoreNode = initNodeText("score", 0.0F, 0.0F, RIGHT, TOP, 60, 12, scoreBehaviour);
  push(&gameScene.nodes, &scoreNode);
  stageNode = initNodeSprite("stage", 500.0F, 10.0F, BLACK_IMAGE, BLACK_IMAGE);
  stageNode.position[1] = -50.0F;//-300.0F;
  stageNode.collisionMaskActive = STAGE_COLLISIONMASK;
  push(&gameScene.nodes, &stageNode);
}

int main(int argc, char *argv[]) {
  initCNSG(argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT);
  initGame();
  startGame();
  gameLoop(FRAME_PER_SECOND);
  deinitCNSG();
  return 0;
}
