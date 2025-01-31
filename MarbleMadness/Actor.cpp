//if pea hits factory and bot damage
//DO DAMAGE STUFF GODAMNIT

#include "Actor.h"
#include "StudentWorld.h"
class Agent;

//implement dosomethings and look at the damage?
//implement marble bepushedby
//look at how to differentiate goodies

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR

Actor::Actor(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir):GraphObject(imageID, startX, startY, startDir){
    m_hp = hitPoints;
    setVisible(true);
    m_world = world;
}

bool Actor::isAlive() const{
    if(m_hp<=0){
        return false;
    }
    return true;
}

void Actor::setDead(){
    
    m_hp=0;
}

void Actor::decHitPoints(int amt){
    m_hp-=amt;
}

StudentWorld* Actor::getWorld() const{
    return m_world;
}

bool Actor::allowsAgentColocation() const{
    return false;
}

bool Actor::allowsMarble() const{
    return false; //WHAT ALLOWS A MARBLE COLOACTION???
}

bool Actor::countsInFactoryCensus() const{
    return false;
}

bool Actor::stopsPea() const{
    return false;
}

bool Actor::isDamageable() const{
    return false;
}

void Actor::damage(int damageAmt){
    m_hp-=damageAmt;
}

bool Actor::bePushedBy(Agent* a, int x, int y){
    return false;
}

bool Actor::isSwallowable() const{
    return false;
}

bool Actor::isStealable() const{
    return false;
}

int Actor::getHitPoints() const{
    return m_hp;
}

void Actor::setHitPoints(int amt){
    m_hp = amt;
}

//bool Actor::tryToBeKilled(int damageAmt){
//    if(isDamageable()){
//        damage(damageAmt);
//        return !isAlive();
//    }
//    else{
//        return false;
//    }
//}

//WALL

Wall::Wall(StudentWorld* world, int startX, int startY): Actor(world, startX, startY, IID_WALL, 1, none){
}

void Wall::doSomething(){
}

bool Wall::stopsPea() const{
    return true;
}

//AGENT
Agent::Agent(StudentWorld* world, int startX, int startY, int imageID,
             int hitPoints, int startDir): Actor(world, startX, startY, imageID, hitPoints, startDir){
}

bool Agent::canMoveInDirection(int dir) {
    switch (dir) {
        case 90:
            return getWorld()->canAgentMoveTo(this, getX(), getY(), 0, 1);
        case 270:
            return getWorld()->canAgentMoveTo(this, getX(), getY(), 0, -1);
        case 180:
            return getWorld()->canAgentMoveTo(this, getX(), getY(), -1, 0);
        case 0:
            return getWorld()->canAgentMoveTo(this, getX(), getY(), 1, 0);
        default:
            return false;
    }
}

void Agent::moveInDirection(int dir){
    switch(dir){
        case 90:
            moveTo(getX(),getY()+1);
            break;
        case 270:
            moveTo(getX(),getY()-1);
            break;
        case 180:
            moveTo(getX()-1,getY());
            break;
        case 0:
            moveTo(getX()+1,getY());
            break;
        default:
            break;
    }
}

//bool Agent::moveIfPossible(){
//    int key;
//    if(getWorld()->getKey(key)){
//        if(key==KEY_PRESS_UP){
//            setDirection(up);
//            if(!getWorld()->canAgentMoveTo(this, getX(), getY(), 0, 1)){
//                moveTo(getX(), getY()+1);
//                return true;
//            }
//        }
//        if(key==KEY_PRESS_DOWN){
//            setDirection(down);
//            if(!getWorld()->canAgentMoveTo(this, getX(), getY(), 0, -1)){
//                moveTo(getX(), getY()-1);
//                return true;
//            }
//        }
//        if(key==KEY_PRESS_LEFT){
//            setDirection(left);
//            if(!getWorld()->canAgentMoveTo(this, getX(), getY(), -1, 0)){
//                moveTo(getX()-1, getY());
//                return true;
//            }
//        }
//        if(key==KEY_PRESS_RIGHT){
//            setDirection(right);
//            if(!getWorld()->canAgentMoveTo(this, getX(), getY(), 1, 0)){
//                moveTo(getX()+1, getY());
//                return true;
//            }
//        }
//    }
//    return false;
//}

bool Agent::moveIfPossible() {
    switch (getDirection()) {
        case 90:
            if(getWorld()->canAgentMoveTo(this, this->getX(), this->getY(), 0, 1)){
                moveTo(getX(), getY() + 1);
                return true;
            }
            break;
        case 270:
            if(getWorld()->canAgentMoveTo(this, this->getX(), this->getY(), 0, -1)){
                moveTo(getX(), getY() - 1);
                return true;
            }
            break;
        case 180:
            if(getWorld()->canAgentMoveTo(this, this->getX(), this->getY(), -1, 0)){
                moveTo(getX() - 1, getY());
                return true;
            }
            break;
        case 0:
            if(getWorld()->canAgentMoveTo(this, this->getX(), this->getY(), 1, 0)){
                moveTo(getX() + 1, getY());
                return true;
            }
            break;
    }
    return false;
}

bool Agent::canPushMarbles() const{
    return false;
}

bool Agent::needsClearShot() const{
    return true;
}

int Agent::shootingSound() const{
    return SOUND_PLAYER_FIRE;
}

//Player
Player::Player(StudentWorld* world, int startX, int startY):Agent(world, startX, startY, IID_PLAYER, 20, right){
    m_peas = 20;
}

void Player::doSomething(){
    if(!isAlive()){
        return;
    }
    int key;
    getWorld()->getKey(key);
    switch(key)
    {
        case KEY_PRESS_ESCAPE:{
            setDead();
            break;
        }
        case KEY_PRESS_SPACE:{
            if(m_peas>0){
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                m_peas--;
                if(getDirection()==90){
                    Actor* tempPea = new Pea(getWorld(), getX(), getY()+1,getDirection());
                    getWorld()->addActor(tempPea);
                }
                if(getDirection()==180){
                    Actor* tempPea = new Pea(getWorld(), getX()-1, getY(),getDirection());
                    getWorld()->addActor(tempPea);
                }
                if(getDirection()==270){
                    Actor* tempPea = new Pea(getWorld(), getX(), getY()-1,getDirection());
                    getWorld()->addActor(tempPea);
                }
                if(getDirection()==0){
                    Actor* tempPea = new Pea(getWorld(), getX()+1, getY(),getDirection());
                    getWorld()->addActor(tempPea);
                }
            }
            else{
                break;
            }
            break;
        }
        case KEY_PRESS_UP:
            setDirection(90);
            getWorld()->pushMarbleIfPossible(getX(), getY() + 1, getX(), getY() + 2);
                moveIfPossible();
            break;
        case KEY_PRESS_DOWN:
            setDirection(270);
            getWorld()->pushMarbleIfPossible(getX(), getY() - 1, getX(), getY() - 2);
                moveIfPossible();
            break;
        case KEY_PRESS_LEFT:
            setDirection(180);
            getWorld()->pushMarbleIfPossible(getX() - 1, getY(), getX() - 2, getY());
                moveIfPossible();
            break;
        case KEY_PRESS_RIGHT:
            setDirection(0);
            getWorld()->pushMarbleIfPossible(getX() + 1, getY(), getX() + 2, getY());
                moveIfPossible();
            break;
        }
    }


bool Player::isDamageable() const{
    return true;
}

void Player::damage(int damageAmt){
    Actor::damage(damageAmt);
    getWorld()->playSound(SOUND_PLAYER_IMPACT);
    if(getHitPoints()<=0){
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
}

bool Player::canPushMarbles() const{
    return true;
}

bool Player::needsClearShot() const{
    return false;
}

int Player::shootingSound() const{
    return SOUND_PLAYER_FIRE;
}

int Player::getHealthPct() const{
    double d = getHitPoints()*100;
    return d/20;
}

int Player::getAmmo() const{
    return m_peas;
}

void Player::restoreHealth(){
    setHitPoints(20);
}

void Player::increaseAmmo(int n){
    m_peas+=n;
}

void Player::setDead(){
    getWorld()->setCrystals(0);
    Actor::setDead();
}

//ROBOT?
Robot::Robot(StudentWorld* world, int startX, int startY, int imageID,
             int hitPoints, int score, int startDir): Agent(world, startX, startY, imageID, hitPoints, startDir){
    m_ticks = 7;
    m_count = 1;
    m_score = score;
}

void Robot::increaseCount(){
    m_count++;
}

void Robot::doSomething() const{
    if(!isAlive()){
        return;
    }
}

bool Robot::isDamageable() const{
    return true;
}

void Robot::damage(int damageAmt){
    Actor::damage(damageAmt);
    if(getHitPoints()>0){
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    if(getHitPoints()<=0){
        getWorld()->increaseScore(m_score);
        getWorld()->playSound(SOUND_ROBOT_DIE);
    }
}

bool Robot::canPushMarbles() const{
    return false;
}

bool Robot::needsClearShot() const{
    return true;
}

int Robot::shootingSound() const{
    return SOUND_ENEMY_FIRE;
}

bool Robot::isShootingRobot() const{
    return true;
}

int Robot::getCount(){
    return m_count;
}

void Robot::setTicks(int t){
    m_ticks = t;
}

//RageBot

RageBot::RageBot(StudentWorld* world, int startX, int startY, int startDir):Robot(world, startX, startY, IID_RAGEBOT, 10, 100, startDir){
}

void RageBot::doSomething(){
    Robot::doSomething();
    int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current
    setTicks(ticks);
    if(ticks < 3){
         setTicks(3); // no RageBot moves more frequently than this
    }
    if(getCount()%ticks!=0){
        increaseCount();
        return;
    }
    if(getDirection()==0){
        if(getWorld()->existsClearShotToPlayer(getX(),getY(),1,0)){
            Actor* tempPea = new Pea(getWorld(), getX()+1, getY(),getDirection());
            getWorld()->addActor(tempPea);
            getWorld()->playSound(SOUND_ENEMY_FIRE);
        }
        else if(getWorld()->canAgentMoveTo(this, getX(), getY(), 1, 0)){
            moveTo(getX()+1,getY());
        }
        else{
            setDirection(180);
        }
    }
    if(getDirection()==90){
        if(getWorld()->existsClearShotToPlayer(getX(),getY(),0,1)){
            Actor* tempPea = new Pea(getWorld(), getX(), getY()+1,getDirection());
            getWorld()->addActor(tempPea);
            getWorld()->playSound(SOUND_ENEMY_FIRE);
        }
        else if(getWorld()->canAgentMoveTo(this, getX(), getY(), 0, 1)){
            moveTo(getX(),getY()+1);
        }
        else{
            setDirection(270);
        }
    }
    if(getDirection()==180){
        if(getWorld()->existsClearShotToPlayer(getX(),getY(),-1,0)){
            Actor* tempPea = new Pea(getWorld(), getX()-1, getY(),getDirection());
            getWorld()->addActor(tempPea);
            getWorld()->playSound(SOUND_ENEMY_FIRE);
        }
        else if(getWorld()->canAgentMoveTo(this, getX(), getY(), -1, 0)){
            moveTo(getX()-1,getY());
        }
        else{
            setDirection(0);
        }
    }
    if(getDirection()==270){
        if(getWorld()->existsClearShotToPlayer(getX(),getY(),0,-1)){
            Actor* tempPea = new Pea(getWorld(), getX(), getY()-1,getDirection());
            getWorld()->addActor(tempPea);
            getWorld()->playSound(SOUND_ENEMY_FIRE);
        }
        else if(getWorld()->canAgentMoveTo(this, getX(), getY(), 0, -1)){
            moveTo(getX(),getY()-1);
        }
        else{
            setDirection(90);
        }
    }
    increaseCount();
}

//ThiefBot
ThiefBot::ThiefBot(StudentWorld* world, int startX, int startY, int imageID,
                   int hitPoints, int score): Robot(world, startX, startY, imageID, 5, 10, right){
    setStatus(false);
    setRandomDistance();
    distanceTraveled = 0;
    pickedUpGoodie = nullptr;
    hasPickedUpGoodie = false;
}

bool ThiefBot::countsInFactoryCensus() const{
    return true;
}

void ThiefBot::damage(int damageAmt){
    Robot::damage(damageAmt);
    if(getHitPoints()<=0){
        if(pickedUpGoodie!=nullptr){
            pickedUpGoodie->setVisible(true);
            pickedUpGoodie->moveTo(getX(), getY());
        }
    }
}

void ThiefBot::doSomething() {
    if (!isAlive()) {
        return;
    }
    Actor* tempGoodie = getWorld()->getColocatedStealable(getX(), getY());
    if (tempGoodie != nullptr && !hasPickedUpGoodie) {
        if(tempGoodie->isVisible()){
            int randomNumber = rand() % 10;
            if (randomNumber == 0) {
                hasPickedUpGoodie = true;
                tempGoodie->setVisible(false);
                pickedUpGoodie = tempGoodie;
                getWorld()->playSound(SOUND_ROBOT_MUNCH);
                increaseCount();
                return;
            }
        }
    }
    if (getDistance() < getRandomDistance()&&canMoveInDirection(getDirection())) {
        if (canMoveInDirection(getDirection())) {
            moveInDirection(getDirection());
            increaseCount();
            increaseDistance();
            return;
        }
    } else {
        setRandomDistance();
        setNewRandomDirection();
        bool moved = false;
        for (int i = 0; i < 4; ++i) {
            if (canMoveInDirection(getDirection())) {
                moveInDirection(getDirection());
                moved = true;
                break;
            }
            setDirection((getDirection() + 90) % 360);
        }
        if (!moved) {
            setDirection((getDirection() + 90) % 360);
        }
    }
    increaseCount();
}


void ThiefBot::setStatus(bool b){
    hasPickedUpGoodie = b;
}

bool ThiefBot::getStatus(){
    return hasPickedUpGoodie;
}

int ThiefBot::getDistance(){
    return distanceTraveled;
}

void ThiefBot::increaseDistance(){
    distanceTraveled++;
}

void ThiefBot::resetDistance(){
    distanceTraveled = 0;
}

void ThiefBot::setNewRandomDirection() {
    int dir = rand() % 4;
    switch (dir) {
        case 0: setDirection(up); break;
        case 1: setDirection(down); break;
        case 2: setDirection(left); break;
        case 3: setDirection(right); break;
    }
}

int ThiefBot::getRandomDistance(){
    return distanceBeforeTurning;
}

void ThiefBot::setRandomDistance(){
    distanceBeforeTurning = rand() % 6 + 1;
}

//RegularThiefBot
RegularThiefBot::RegularThiefBot(StudentWorld* world, int startX, int startY): ThiefBot(world, startX, startY, IID_THIEFBOT, 5, 10){
}

bool RegularThiefBot::isShootingRobot() const{
    return false;
}

//make sure put goodie back when die
void RegularThiefBot::doSomething(){
    if(!isAlive()){
        return;
    }
    int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current
    setTicks(ticks);
    if(ticks < 3){
         setTicks(3); // no RageBot moves more frequently than this
    }
    if(getCount()%ticks!=0){
        increaseCount();
        return;
    }
    ThiefBot::doSomething();
}

//MeanThiefBot
MeanThiefBot::MeanThiefBot(StudentWorld* world, int startX, int startY):ThiefBot(world, startX, startY, IID_MEAN_THIEFBOT, 8, 20){
}

void MeanThiefBot::doSomething() {
    if(!isAlive()){
        return;
    }
    int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current
    setTicks(ticks);
    if(ticks < 3){
         setTicks(3); // no RageBot moves more frequently than this
    }
    if(getCount()%ticks!=0){
        increaseCount();
        return;
    }
    switch (getDirection()) {
        case right:
            if (getWorld()->existsClearShotToPlayer(getX(), getY(), 1, 0)) {
                Actor* tempPea = new Pea(getWorld(), getX() + 1, getY(), getDirection());
                getWorld()->addActor(tempPea);
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                increaseCount();
                return;
            }
            break;
        case up:
            if (getWorld()->existsClearShotToPlayer(getX(), getY(), 0, 1)) {
                Actor* tempPea = new Pea(getWorld(), getX(), getY() + 1, getDirection());
                getWorld()->addActor(tempPea);
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                increaseCount();
                return;
            }
            break;
        case left:
            if (getWorld()->existsClearShotToPlayer(getX(), getY(), -1, 0)) {
                Actor* tempPea = new Pea(getWorld(), getX() - 1, getY(), getDirection());
                getWorld()->addActor(tempPea);
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                increaseCount();
                return;
            }
            break;
        case down:
            if (getWorld()->existsClearShotToPlayer(getX(), getY(), 0, -1)) {
                Actor* tempPea = new Pea(getWorld(), getX(), getY() - 1, getDirection());
                getWorld()->addActor(tempPea);
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                increaseCount();
                return;
            }
            break;
    }
        // If no shot was fired, proceed with the rest of the actions for the MeanThiefBot
        ThiefBot::doSomething();
}

//Exit
Exit::Exit(StudentWorld* world, int startX, int startY):Actor(world, startX, startY, IID_EXIT, 1, none){
    setVisible(false);
}

bool Exit::allowsAgentColocation() const{
    return true;
}

void Exit::doSomething(){
    if(!getWorld()->anyCrystals()){
        if(!isVisible()){
            getWorld()->playSound(SOUND_REVEAL_EXIT);
        }
        setVisible(true);
    }
    if(isVisible()){
        if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()) {
            getWorld()->playSound(SOUND_FINISHED_LEVEL);
                getWorld()->setLevelFinished();
        }
    }
}

//Marble
Marble::Marble(StudentWorld* world, int startX, int startY): Actor(world, startX, startY, IID_MARBLE, 10, none){
    
}

bool Marble::isDamageable() const{
    return true;
}

void Marble::damage(int damageAmt){
    Actor::damage(damageAmt);
}

bool Marble::isSwallowable() const{
    return true;
}

//IMPLEMENT THIS LATER
bool Marble::bePushedBy(Agent* a, int x, int y){
    if(getWorld()->canMarbleMoveTo(x, y)&&a->canPushMarbles()){
        return true;
    }
    return false;
}

void Marble::doSomething(){
}

//Pit
Pit::Pit(StudentWorld* world, int startX, int startY):Actor(world, startX, startY, IID_PIT, 1, none){
    
}

bool Pit::allowsMarble() const{
    return true;
}

void Pit::doSomething(){
    if(!isAlive()){
        return;
    }
    if(getWorld()->isMarbleColocatedWith(getX(), getY())){
        setDead();
    }
}

//Pea
Pea::Pea(StudentWorld* world, int startX, int startY, int startDir):Actor(world, startX, startY, IID_PEA, 1, startDir){
}

bool Pea::allowsAgentColocation() const{
    return true;
}

void Pea::doSomething(){
    if(!isAlive()){
        return;
    }
    if (getWorld()->damageSomething(this, 2)) {
        setDead();
        return;
    }
    switch(getDirection()){
        case left:
            if(getWorld()->isPeaColocatedWith(this, getX() - 1, getY())){
                setVisible(false);
            }
            break;
        case right:
            if(getWorld()->isPeaColocatedWith(this, getX() + 1, getY())){
                setVisible(false);
            }
            break;
        case up:
            if(getWorld()->isPeaColocatedWith(this, getX(), getY() + 1)){
                setVisible(false);
            }
            break;
        case down:
            if(getWorld()->isPeaColocatedWith(this, getX(), getY() - 1)){
                setVisible(false);
            }
            break;
    }
    if(getWorld()->isPeaColocatedWith(this, getX(), getY())){
        setDead();
        return;
    }
    // Move the pea based on its direction
    switch(getDirection()) {
        case right:
            moveTo(getX() + 1, getY());
            break;
        case left:
            moveTo(getX() - 1, getY());
            break;
        case up:
            moveTo(getX(), getY() + 1);
            break;
        case down:
            moveTo(getX(), getY() - 1);
            break;
    }

    // Check for collisions and deal damage at the new position
    if (getWorld()->damageSomething(this, 2)) {
        setDead();
        return;
    }
    if(getWorld()->isPeaColocatedWith(this, getX(), getY())){
        setDead();
        return;
    }
}

//PIckupabalbsefasitem
PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score):Actor(world, startX, startY, imageID, 1, none){
    this->score = score;
}

bool PickupableItem::allowsAgentColocation() const{
    return true;
}

void PickupableItem::doSomething(){
    
}

int PickupableItem::getScore(){
    return score;
}

//crystal!
Crystal::Crystal(StudentWorld* world, int startX, int startY):PickupableItem(world, startX, startY, IID_CRYSTAL, 50){
}

void Crystal::doSomething(){
    if(!isAlive()){
        return;
    }
    if(getWorld()->isPlayerColocatedWith(getX(), getY())){
        getWorld()->increaseScore(getScore());
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
        getWorld()->decCrystals(1);
    }
}


//Goodie???????
Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID,
               int score):PickupableItem(world, startX, startY, imageID, score){
}

bool Goodie::isStealable() const{
    return true;
}

void Goodie::setStolen(bool status){
    isStolen = status;
}

void Goodie::doSomething(){
    if(!isAlive()){
        return;
    }
    if(getWorld()->isPlayerColocatedWith(getX(), getY())){
        getWorld()->playSound(SOUND_GOT_GOODIE);
        if(getScore()==1000){
            getWorld()->increaseScore(1000);
            setDead();
            getWorld()->incLives();
        }
        if(getScore()==500){
            getWorld()->increaseScore(500);
            setDead();
            getWorld()->getPlayer()->restoreHealth();
        }
        if(getScore()==100){
            getWorld()->increaseScore(100);
            setDead();
            getWorld()->getPlayer()->increaseAmmo(20);
        }
    }
}

//extralife
ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY): Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000){
}

//restorehealth
RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int startX, int startY): Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500){
}

//ammo
AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY): Goodie(world, startX, startY, IID_AMMO, 100){
}

//ThiefBotFactory
ThiefBotFactory::ThiefBotFactory(StudentWorld* world, int startX, int startY, ProductType type):Actor(world, startX, startY, IID_ROBOT_FACTORY, 1, none){
    t = type;
    isActive = true;
}

bool ThiefBotFactory::stopsPea() const{
    return true;
}

void ThiefBotFactory::doSomething(){
    int count = 0;
    if(getWorld()->doFactoryCensus(getX(), getY(), 3, count)){
        if(!getWorld()->isThiefBotOnFactory(getX(), getY())){
            int spawnChance = rand() % 50; // Generate a random number between 0 and 49
            if (spawnChance == 0) { // 1 in 50 chance
                // Spawn a ThiefBot at the factory's location
                if(t==MEAN){
                    getWorld()->addActor(new MeanThiefBot(getWorld(), getX(), getY()));
                    getWorld()->playSound(SOUND_ROBOT_BORN);
                }
                if(t==REGULAR){
                    getWorld()->addActor(new RegularThiefBot(getWorld(), getX(), getY()));
                    getWorld()->playSound(SOUND_ROBOT_BORN);
                }
            }
        }
    }
}
