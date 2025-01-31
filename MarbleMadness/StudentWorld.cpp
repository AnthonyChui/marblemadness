#include <algorithm>
#include "StudentWorld.h"
#include "Level.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

void StudentWorld::addActor(Actor* a){
    actors.push_back(a);
}

int StudentWorld::init()
{
    m_nCrystals = 0;
    bonus = 1000;
    Level lev(assetPath());
    std::string curLevel;
    if(getLevel() > 9)
    {
        curLevel = "level" + std::to_string(getLevel()) + ".txt";
    }
    else{
        curLevel = "level0" + std::to_string(getLevel()) + ".txt";
    }
    Level::LoadResult result = lev.loadLevel(curLevel);
    if(result==Level::load_fail_file_not_found && getLevel()>99){
        return GWSTATUS_PLAYER_WON;
    }
    else if(result == Level::load_fail_bad_format){
        return GWSTATUS_LEVEL_ERROR;
    }
    if (result == Level::load_fail_file_not_found){
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level\n";
        for(int i = 0; i < 15; i++){
            for(int j = 0; j < 15; j++){
                Level::MazeEntry ge = lev.getContentsOf(i,j); // x=5, y=10
                switch (ge)
                {
                    case Level::empty:
                    {
                        break;
                    }
                    case Level::player:{
                        m_player = new Player(this, i, j);
                        addActor(m_player);
                        break;
                    }
                    case Level::exit:
                    {
                        Actor* temp = new Exit(this, i, j);
                        addActor(temp);
                        break;
                    }
                    case Level::wall:{
                        Actor* temp = new Wall(this, i, j);
                        addActor(temp);
                        break;
                    }
                    case Level::horiz_ragebot: {
                        Actor* temp = new RageBot(this, i, j, 0);
                        addActor(temp);
                        break;
                    }
                    case Level::vert_ragebot: {
                        Actor* temp = new RageBot(this, i, j, 270);
                        addActor(temp);
                        break;
                    }
                    case Level::thiefbot_factory: {
                        Actor* temp = new ThiefBotFactory(this, i, j, ThiefBotFactory::REGULAR);
                        addActor(temp);
                        break;
                    }
                    case Level::mean_thiefbot_factory: {
                        Actor* temp = new ThiefBotFactory(this, i, j, ThiefBotFactory::MEAN);
                        addActor(temp);
                        break;
                    }
                    case Level::marble: {
                        Actor* temp = new Marble(this, i, j);
                        addActor(temp);
                        break;
                    }
                    case Level::pit: {
                        Actor* temp = new Pit(this, i, j);
                        addActor(temp);
                        break;
                    }
                    case Level::crystal: {
                        Actor* temp = new Crystal(this, i, j);
                        addActor(temp);
                        m_nCrystals++;
                        break;
                    }
                    case Level::restore_health: {
                        Actor* temp = new RestoreHealthGoodie(this, i, j);
                        addActor(temp);
                        break;
                    }
                    case Level::extra_life: {
                        Actor* temp = new ExtraLifeGoodie(this, i, j);
                        addActor(temp);
                        break;
                    }
                    case Level::ammo: {
                        Actor* temp = new AmmoGoodie(this, i, j);
                        addActor(temp);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    //remove dead actors
    if(bonus>0)
    bonus--;
    for (int i = actors.size() - 1; i >= 0; i--) {
        if (!actors[i]->isAlive()) {
            delete actors[i]; // Delete the actor to free memory
            actors.erase(actors.begin() + i); // Remove the actor from the vector
        }
    }

    // Update each actor
    for (auto actor : actors) {
        actor->doSomething();
        if (!m_player->isAlive()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        if (levelStatus == GWSTATUS_FINISHED_LEVEL) {
            increaseScore(bonus);
            playSound(SOUND_FINISHED_LEVEL);
            levelStatus=GWSTATUS_CONTINUE_GAME;
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    // Update game statistics
    setDisplayText();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::getBonus() const{
    return bonus;
}

void StudentWorld::setDisplayText() {
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    // Assuming you have methods to get the health, ammo, and bonus values
    int health = m_player->getHealthPct();
    int ammo = m_player->getAmmo();
    int bonus = getBonus();

    // Format the display text
    std::string s = "Score: " + std::to_string(score) +
                    " Level: " + std::to_string(level) +
                    " Lives: " + std::to_string(livesLeft) +
                    " Health: " + std::to_string(health) + "%" +
                    " Ammo: " + std::to_string(ammo) +
                    " Bonus: " + std::to_string(bonus);

    // Update the display text at the top of the screen
    setGameStatText(s);
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i < actors.size(); i++){
        delete actors[i];
    }
    actors.clear();
}

bool StudentWorld::canAgentMoveTo(Agent* agent, int x, int y, int dx, int dy) const{
    for(int i = 0; i < actors.size(); i++){
        if(actors[i]->getX()==x+dx&&actors[i]->getY()==y+dy){
            if(!actors[i]->allowsAgentColocation()){
                return false;
            }
        }
    }
    return true;
}

bool StudentWorld::canMarbleMoveTo(int x, int y) const {
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->getX() == x && actors[i]->getY() == y) {
            if (!actors[i]->allowsMarble()) {
                return false; // Marble cannot move if the actor at the destination does not allow it
            }
        }
    }
    return true; // Marble can move if there are no actors blocking the way
}

bool StudentWorld::isPlayerColocatedWith(int x, int y) const{
    if(m_player->getX()==x&&m_player->getY()==y){
        return true;
    }
    return false;
}

//do i delete the pea after?
bool StudentWorld::damageSomething(Actor* a, int damageAmt) {
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->isDamageable()) {
            if (actors[i]->getX() == a->getX() && actors[i]->getY() == a->getY()) {
                actors[i]->damage(damageAmt);
                a->setDead(); // Set the pea actor to dead instead of deleting it
                return true;
            }
        }
    }
    return false;
}


//do i delete the swallowed marble?
bool StudentWorld::swallowSwallowable(Actor* a){
    for(int i = 0; i < actors.size(); i++){
        if(actors[i]->isSwallowable()){
            if(actors[i]->getX()==a->getX()&&actors[i]->getY()==a->getY()){
                delete actors[i];
                return true;
            }
        }
    }
    return false;
}

//wrong also need to check if something is in the way
bool StudentWorld::existsClearShotToPlayer(int x, int y, int dx, int dy) const {
    if (dx == 1) {
        if (m_player->getX() >= x && m_player->getY() == y) {
            for (Actor* actor : actors) {
                if (actor->getX() > x && actor->getX() < m_player->getX() && actor->getY() == y && (actor->stopsPea()||actor->isDamageable())) {
                    return false;
                }
            }
            return true;
        }
    } else if (dx == -1) {
        if (m_player->getX() <= x && m_player->getY() == y) {
            for (Actor* actor : actors) {
                if (actor->getX() < x && actor->getX() > m_player->getX() && actor->getY() == y&&(actor->stopsPea()||actor->isDamageable())) {
                    return false;
                }
            }
            return true;
        }
    } else if (dy == 1) {
        if (m_player->getY() >= y && m_player->getX() == x) {
            for (Actor* actor : actors) {
                if (actor->getY() > y && actor->getY() < m_player->getY() && actor->getX() == x&&(actor->stopsPea()||actor->isDamageable())) {
                    return false;
                }
            }
            return true;
        }
    } else if (dy == -1) {
        if (m_player->getY() <= y && m_player->getX() == x) {
            for (Actor* actor : actors) {
                if (actor->getY() < y && actor->getY() > m_player->getY() && actor->getX() == x&&(actor->stopsPea()||actor->isDamageable())) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}


bool StudentWorld::doFactoryCensus(int x, int y, int distance, int& count) const {
    int minX = std::max(0, x - distance);
    int maxX = std::min(15, x + distance);
    int minY = std::max(0, y - distance);
    int maxY = std::min(15, y + distance);
    count = 0;
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->countsInFactoryCensus()) {
            int actorX = actors[i]->getX();
            int actorY = actors[i]->getY();
            if (actorX >= minX && actorX <= maxX && actorY >= minY && actorY <= maxY) {
                count++;
            }
        }
    }
    return count < 3;
}

Actor* StudentWorld::getColocatedStealable(int x, int y) const{
    for(int i = 0; i < actors.size(); i++){
        if(actors[i]->isStealable()&&actors[i]->getX()==x&&actors[i]->getY()==y){
            return actors[i];
        }
    }
    return nullptr;
}

void StudentWorld::restorePlayerHealth(){
    m_player->restoreHealth();
}

void StudentWorld::increaseAmmo(){
    m_player->increaseAmmo(20);
}

bool StudentWorld::anyCrystals() const{
    //    for(int i = 0; i < actors.size(); i++){
    //        if(!(actors[i]->isStealable())&&actors[i]->allowsAgentColocation()){
    //            return true;
    //        }
    //    }
    //    return false;
    if(m_nCrystals>0)
    {
        return true;
    }
    return false;
}

void StudentWorld::setCrystals(int i){
    m_nCrystals = i;
}

void StudentWorld::decCrystals(int i) {
    m_nCrystals-=i;
}

void StudentWorld::setLevelFinished(){
    levelStatus = GWSTATUS_FINISHED_LEVEL;
}

bool StudentWorld::pushMarbleIfPossible(int oldx, int oldy, int newx, int newy) {
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->getX() == oldx && actors[i]->getY() == oldy && actors[i]->isSwallowable()) {
            if(actors[i]->bePushedBy(m_player, newx, newy)){
                actors[i]->moveTo(newx, newy);
                return true; // Marble successfully pushed
            }
            else{
                return false;
            }
        }
    }
    return false; // No marble found or unable to push
}

Player* StudentWorld::getPlayer(){
    return m_player;
}

//will only be called true if pitted?
bool StudentWorld::isMarbleColocatedWith(int x, int y) const{
    for(int i = 0; i < actors.size(); i++){
        if(actors[i]->isSwallowable()&&actors[i]->getX()==x&&actors[i]->getY()==y){
            actors[i]->setDead();
            return true;
        }
    }
    return false;
}

bool StudentWorld::isPeaColocatedWith(Actor* p, int x, int y){
    for(Actor* actor: actors){
        if((actor->stopsPea()||actor->isDamageable())&&actor->getX()==x&&actor->getY()==y){
            p->setDead();
            return true;
        }
    }
    return false;
}



bool StudentWorld::isThiefBotOnFactory(int factoryX, int factoryY) const {
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->countsInFactoryCensus()) {
            if (actors[i]->getX() == factoryX && actors[i]->getY() == factoryY) {
                return true;
            }
        }
    }
    return false;
}
