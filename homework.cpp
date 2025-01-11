#include<iostream>
#include<cmath>
#include<unistd.h>
using namespace std;

// 基本人物，所有角色（戰士、法師、怪物等）
class Character {
protected:
    static const int EXP_LV = 100;  // ???算的基?常?
    string name;  // 姓名
    int hp;  // 生命值
    int level;  // 等級 
    int exp;  // 經驗
    int power;  // 攻擊力
    int knowledge;  // 法師技能值
    int luck;  // 運气值
    
    // 角色升?函?，增加角色?性
    void levelUp(int hInc, int pInc, int kInc, int lInc);
public:
    // 构造函?，初始化角色?性
    Character(string n, int lv, int h, int po, int kn, int lu);
    virtual void print();  // 打印角色信息
    virtual void beatMonster(int exp) = 0;  // ??怪物，增加??
    virtual void setHP(int) = 0;  // ?置生命值
    virtual int getHP() = 0;  // ?取生命值
    virtual int getAttack() = 0;  // ?取攻?力
    virtual void setPower(int newPower) = 0;  // ?置攻?力
    string getName();  // ?取角色姓名
};

// Character?构造函?的??
Character::Character(string n, int lv, int h, int po, int kn, int lu) : name(n), level(lv), exp(pow(lv - 1, 2) * EXP_LV), hp(h), power(po), knowledge(kn), luck(lu) {}

// ?取角色姓名
string Character::getName() {
    return this->name;
}

// 角色升?函???
void Character::levelUp(int hInc, int pInc, int kInc, int lInc) {
    this->level++;
    this->hp += hInc;
    this->power += pInc;
    this->knowledge += kInc;
    this->luck += lInc;
}

// 打印角色信息
void Character::print() {
    cout << this->name << ": " << this->level << " (" << this->exp << "/" << pow(this->level, 2) * EXP_LV << "), ";
    if(this->hp >= 0)
        cout << this->hp;
    else
        cout << "0";
    cout << "-" << this->power << "-" << this->knowledge << "-" << this->luck << "\n";
}

// ?士?，?承自Character?
class Warrior : public Character {
private:
    static const int HP_LV = 100;  // 每?增加的生命值
    static const int PO_LV = 10;   // 每?增加的攻?力
    static const int KN_LV = 5;    // 每?增加的技能值
    static const int LU_LV = 5;    // 每?增加的?气值
public:
    // 构造函?
    Warrior(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
    
    // 重?打印函?
    void print() { cout << "Warrior "; Character::print(); }
    int getHP() { return hp; }  // ?取生命值
    void setHP(int hp) { this->hp = hp; }  // ?置生命值
    int getAttack() { return power; }  // ?取攻?力
    void setPower(int newPower) { this->power = newPower; }  // ?置攻?力
    // ??怪物并增加??
    void beatMonster(int exp) {
        this->exp += exp;
        while(this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, PO_LV, KN_LV, LU_LV);
    }
};

// 法??，?承自Character?
class Wizard : public Character {
private:
    static const int HP_LV = 80;  // 每?增加的生命值
    static const int PO_LV = 4;   // 每?增加的攻?力
    static const int KN_LV = 15;  // 每?增加的技能值
    static const int LU_LV = 7;   // 每?增加的?气值
public:
    // 构造函?
    Wizard(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
    
    // 重?打印函?
    void print() { cout << "Wizard "; Character::print(); }
    int getHP() { return hp; }  // ?取生命值
    void setHP(int hp) { this->hp = hp; }  // ?置生命值
    int getAttack() { return knowledge; }  // ?取攻?力（法?以知?作?攻?力）
    void setPower(int newPower) { this->power = newPower; }  // ?置攻?力
    // ??怪物并增加??
    void beatMonster(int exp) {
        this->exp += exp;
        while(this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, PO_LV, KN_LV, LU_LV);
    }
};

// 怪物?，?承自Character?
class Monster : public Character {
public:
    // 构造函?
    Monster(int h, int po, int kn) : Character("", 0, h, po, kn, 0) {}
    
    // 重?打印函?
    void print() { cout << "Monster "; Character::print(); }
    int getHP() { return hp; }  // ?取生命值
    void setHP(int hp) { this->hp = hp; }  // ?置生命值
    int getAttack() { return power; }  // ?取攻?力
    void setPower(int newPower) { this->power = newPower; }  // ?置攻?力
    // ??怪物?不需要做任何操作
    void beatMonster(int exp) {}
};

// ?伍?，管理多?角色（?士和法?）
class Team {
protected:
    int memberCount;  // ?伍成??量
    Character* member[10];  // ?伍成?（最多10?）
public:
    Team();  // 构造函?
    ~Team();  // 析构函?
    void addWarrior(string name, int lv);  // 增加?士成?
    void addWizard(string name, int lv);   // 增加法?成?
    void memberBeatMonster(string name, int exp);  // 指定角色??怪物并?得??
    void print();  // 打印?伍信息
    void combat(Monster *m);  // ?始与怪物?斗
    void effect(int a);  // ??效果：增加攻?力、恢复血量等
};

// Team?构造函?的??
Team::Team() {
    memberCount = 0;
    for(int i = 0; i < 10; i++) {
        member[i] = NULL;
    }
}

// Team?析构函?，?放成?的?存
Team::~Team() {
    for(int i = 0; i < memberCount; i++) {
        delete member[i];
    }
}

// 向隊伍中添加戰士
void Team::addWarrior(string name, int lv) {
    if(memberCount < 10) {
        member[memberCount] = new Warrior(name, lv);
        memberCount++;
    }
}

// 向隊伍中添加法師
void Team::addWizard(string name, int lv) {
    if(memberCount < 10) {
        member[memberCount] = new Wizard(name, lv);
        memberCount++;
    }
}

// ?指定的角色??怪物并?得??
void Team::memberBeatMonster(string name, int exp) {
    for(int i = 0; i < memberCount; i++) {
        if(member[i]->getName() == name) {
            member[i]->beatMonster(exp);
        }
    }
}

// 打印隊伍信息
void Team::print() {
    for(int i = 0; i < memberCount; i++) {
        member[i]->print();
    }
}

// 与怪物進行戰斗
void Team::combat(Monster* m) {
    int teamHP = 1000;  // 宣告隊伍的生命值

    cout << "Monster" << " " << m->getHP() << "HP " << m->getAttack() << "Power" << endl;
    sleep(1);

    // 持續戰斗，直到怪物或者隊伍某一方死亡
    while (m->getHP() > 0 && teamHP > 0) {
        int aliveMemberCount = 0;
        for (int i = 0; i < memberCount; i++) {
            if (member[i]->getHP() > 0) {
                aliveMemberCount++;  // 存活的隊伍成員 
            }
        }
        sleep(1);

        // 怪物攻擊隊伍
        for (int i = 0; i < memberCount; i++) {
            if (member[i]->getHP() > 0) {
                member[i]->setHP(member[i]->getHP() - m->getAttack() / aliveMemberCount);  // 平均分配怪物的攻?
                cout << "Monster attacks " << m->getAttack() / aliveMemberCount << " to " << member[i]->getName() << endl;
                sleep(1);
            }
        }

        // 隊伍攻擊怪物
        for (int i = 0; i < memberCount; i++) {
            if (member[i]->getHP() > 0) {
                m->setHP(m->getHP() - member[i]->getAttack());  // 扣除怪物的生命
                cout << member[i]->getName() << " attacks: " << member[i]->getAttack() << " to Monster" << endl;
                sleep(1);
            }
        }

        cout << "Monster HP: " << m->getHP() << endl;

        // ?算?伍的?生命值
        teamHP = 0;
        for (int i = 0; i < memberCount; i++) {
            if (member[i]->getHP() > 0) {
                teamHP += member[i]->getHP();
            }
            cout << member[i]->getName() << "'s HP: " << member[i]->getHP() << endl;
            sleep(1);
        }

        // 如果怪物被打敗 
        if (m->getHP() <= 0) {
            cout << "Win!" << endl;
            cout << "Player Status:\n";
            print();
            
            // 增加怪物難度 
            int a;
            int newHP = m->getHP() + 500;
            int newPower = m->getAttack() + static_cast<int>(m->getAttack() * 0.2);
            int newKnowledge = 500;
            Monster mon(newHP, newPower, newKnowledge);
            sleep(1);
            
            cout << "Choose effect: 1. for increase attack power  2. for restore HP  3. increase attack power but lose HP  4. increase HP but lose Power" << endl;
            while (true) {
                if (!(cin >> a)) {  // ?查cin?入是否有效
                    cin.clear();  // 清除???志
                    continue;
                }
                effect(a);  // ?用效果函?
                cout << "Next Level\n";
                combat(&mon);  // 再次進入戰鬥
                return;
            }
        } 
        // 如果隊伍被打敗 
        else if (teamHP <= 0) {
            cout << "Lose" << endl;
            m->print();
            cout << "Game over" << endl;
            break;// 遊戲結束 
        }
    }
}

// 使用戰鬥后的效果
void Team::effect(int a) {
    if (a == 1) {  // 增加攻擊力
        for (int i = 0; i < memberCount; i++) {
            if (member[i]->getHP() > 0) {
                int newAttack = member[i]->getAttack() + 100;
                member[i]->setPower(newAttack);  // 增加攻擊力
                cout << member[i]->getName() << " has gained 100 attack power!" << endl;
                sleep(1);
            }
        }
    } else if (a == 2) {  // 恢复生命值
        for (int i = 0; i < memberCount; i++) {
            if (member[i]->getHP() > 0) {
                member[i]->setHP(member[i]->getHP() + 100);  // 恢复100生命值
                cout << member[i]->getName() << " has regained 100 HP!" << endl;
                sleep(1);
            }
        }
    }
    else if (a == 3) {  // 犧牲生命增加攻擊力
        for (int i = 0; i < memberCount; i++) {
            if (member[i]->getHP() > 0) {
                member[i]->setHP(member[i]->getHP() - 70);  // 失去70生命
                int newAttack = member[i]->getAttack() + 200;
                member[i]->setPower(newAttack);  // 增加攻擊力
                cout << member[i]->getName() << " Lost 70 HP, gained 200 attack power!" << endl;
                sleep(1);
            }
        }
    }
    else if (a == 4) {  // 犧牲攻擊力恢复更多生命
        for (int i = 0; i < memberCount; i++) {
            if (member[i]->getHP() > 0) {
                member[i]->setHP(member[i]->getHP() + 250);  // 增加250生命值
                int newAttack = member[i]->getAttack() - 70;  // 減少攻擊力
                member[i]->setPower(newAttack);  // 設置新的攻擊力
                cout << member[i]->getName() << " Lost 70 attack, gained 250 HP!" << endl;
                sleep(1);
            }
        }
    }
}

// 主函式 
int main() {
    int mHP = 200, mPower = 100, mKnowledge = 500;
    Monster mon(mHP, mPower, mKnowledge);  // 創建怪物
    Team myTeam;  // 創建隊伍 

    string name;
    int lv = 0, role = 0, memberCount = 0;

    cout << "Set your character: 1.Warrior 2.Wizard "<<endl<<"If you finish, press ctrl+z: "<<endl;
    // 輸入隊伍 
    while(!cin.eof()) {
        name = "";
        if(memberCount < 10) {
            cin >> name >> lv >> role;
            if(name == "") break;
            if(role == 1) {
                myTeam.addWarrior(name, lv);  // 添加戰士
                memberCount++;
            } else if(role == 2) {
                myTeam.addWizard(name, lv);  // 添加法師
                memberCount++;
            }
        }
    }

    myTeam.combat(&mon);  // 開始戰斗

    return 0;
}
