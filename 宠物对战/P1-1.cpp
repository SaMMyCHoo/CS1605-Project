#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
using namespace std;
class Pet
{
    public:
    Pet()
    {
        HP[1]=110,HP[2]=100,HP[3]=100;
        ATK[1]=10,ATK[2]=11,ATK[3]=10;
        DF[1]=10,DF[2]=10,DF[3]=11;
        now_up=0;
    }
    void UP(int pet)
    {
        now_up=pet;
    }
    int get_UP() const
    {
        return now_up;
    }
    int get_HP() const
    {
        return HP[now_up];
    }
    int deadcnt() const
    {
        int cnt=0;
        if(HP[1]<=0)
            ++cnt;
        if(HP[2]<=0)
            ++cnt;
        if(HP[3]<=0)
            ++cnt;
        return cnt;
    }
    string choices(int now) const
    {//1 for W, 2 for L, 3 for G
        if(deadcnt()==0||(deadcnt()==1&&!alive(now)))
        {
            if(now==1)
                return "2 for L, 3 for G";
            if(now==2)
                return "1 for W, 3 for G";
            if(now==3)
                return "1 for W, 2 for L";
        }
        else if(deadcnt()==1||deadcnt()==2&&!alive(now))
        {
            if(now!=1&&alive(1))
                return "1 for W";
            if(now!=2&&alive(2))
                return "2 for L";
            if(now!=3&&alive(3))
                return "3 for G";
        }
        else//理论上不存在
            return "";
    } 
    char UP_str() const
    {
        if(now_up==1)
            return 'W';
        if(now_up==2)
            return 'L';
        return 'G';
    }
    string SKL_str(int op) const
    {
        if(op==1)
            return "Tackle";
        if(now_up==1)
            return "Leaf";
        else if(now_up==2)
            return "Flame";
        return "Stream";
    }
    bool alive(int pet) const
    {
        return HP[pet]>0;
    }
    bool lost() const
    {
        if(HP[1]>0)
            return false;
        if(HP[2]>0)
            return false;
        if(HP[3]>0)
            return false;
        return true;
    }
    bool advantage(int enemy) const
    {
        if(enemy==1)
        {
            if(now_up==2)
                return true;
            return false;
        }
        if(enemy==2)
        {
            if(now_up==3)
                return true;
            return false;
        }
        //enemy=3
        if(now_up==1)
            return true;
        return false;
    }
    int get_damage(int atk,int enemy) const
    {
        double res=20.0*ATK[now_up]/DF[enemy];
        if(atk==1)
            return int(res+0.5);
        if(advantage(enemy))
            return int(0.5+res*2);
        return int(0.5+res*0.5);
    }
    void attacked(int damage)
    {
        HP[now_up]-=damage;
    }
    private:
    //所有攻击方式威力统一为20，故不单独储存。
    int HP[4],DF[4],ATK[4];
    int now_up;
};
Pet player,enemy;
struct operation
{
    int choice,obj;
    /*
        choice:
        0 换人
        1 进攻
        2 吃药
    */

};
void init(ostream &os,istream &is);
void play(ostream &os,istream &is);
int get_level(operation x);
void print(ostream &os,int side,operation op,Pet* x,Pet* y);
void carry(operation op,Pet* x,Pet* y);
int main()
{
    srand(time(0));
    init(cout,cin);
    play(cout,cin);
    return 0;
}
void init(ostream &os,istream &is)
{
    os<<"Welcome to Battle of Pets!"<<endl;
    os<<"You have W, L and G. So does Enemy."<<endl;
    while(true)
    {
        os<<"Select your starting pet (1 for W, 2 for L, 3 for G):";
        int player_choice;
        is>>player_choice;
        if(player_choice==1||player_choice==2||player_choice==3)
        {
            player.UP(player_choice);
            break;
        }
        else
            continue;
    }
    os<<"You start with "<<player.UP_str()<<endl;
    int enemy_choice=rand()%3+1;
    enemy.UP(enemy_choice);
    os<<"Enemy starts with "<<enemy.UP_str()<<endl;
    os<<"Battle starts!"<<endl;
    return;
}
int get_level(operation x)
{
    if(x.choice==0)
        return 6;
    if(x.choice==1)
        return 0;
    if(x.choice==2)
        return 5;
}
void print(ostream &os,int side,operation op,Pet& x,Pet& y)
{
    if(!side)
    {
        if(op.choice==0)
            os<<"You send "<<x.UP_str()<<endl;
        else
        {
            int damage=x.get_damage(op.obj,y.get_UP());
            os<<"Your "<<x.UP_str()<<" uses "<<x.SKL_str(op.obj)<<"! Damage: "<<damage<<endl;
        }
    }
    else
    {
        if(op.choice==0)
            os<<"Enemy send "<<x.UP_str()<<endl;
        else
        {
            int damage=x.get_damage(op.obj,y.get_UP());
            os<<"Enemy's "<<x.UP_str()<<" uses "<<x.SKL_str(op.obj)<<"! Damage: "<<damage<<endl;
        }      
    }
}
void carry(operation op,Pet& x,Pet& y)
{
    if(op.choice==0)
    {
        x.UP(op.obj);
        return;
    }
    if(op.choice==1)
    {
        int damage=x.get_damage(op.obj,y.get_UP());
        y.attacked(damage);
        return;
    }
}
void play(ostream &os,istream &is)
{
    int round=0;
    while(true)
    {
        os<<"--------------------------------------------------"<<endl;
        os<<"Round "<<++round<<endl;
        operation now,Enow;
        while(true)
        {
            os<<"Select your action ";
            int choice;
            if(player.deadcnt()<2)
            {
                os<<"(0 for change, 1 for skill): ";               
                is>>choice;
                if(choice!=0&&choice!=1)
                    continue;
            }
            else
            {
                os<<"(1 for skill): ";
                is>>choice;
                if(choice!=1)
                    continue;
            }
            if(choice==0)
            {
                if(player.deadcnt()==2)
                    continue;
                now.choice=0;
                while(true)
                {
                    os<<"Select your next pet (";
                    os<<player.choices(player.get_UP());
                    os<<"): ";
                    int nxt;
                    is>>nxt;
                    if(nxt!=player.get_UP()&&player.alive(nxt))
                    {
                        now.obj=nxt;
                        break;
                    }
                    else
                        continue;
                }
                break;
            }
            else
            {
                now.choice=1;
                while(true)
                {
                    os<<"Select the skill (1 for Tackle, 2 for "<<player.SKL_str(2)<<"): ";
                    int atk;
                    is>>atk;
                    if(atk==1||atk==2)
                    {
                        now.obj=atk;
                        break;
                    }
                    else
                        continue;
                }
                break;
            }
        }
        int player_level=get_level(now);
        if(enemy.advantage(player.get_UP()))//敌方克制我方
            Enow.choice=1,Enow.obj=2;
        else
            Enow.choice=1,Enow.obj=1;
        int enemy_level=get_level(Enow);
        if(enemy_level>player_level)
        {
            carry(Enow,enemy,player);
            carry(now,player,enemy);
            print(os,1,Enow,enemy,player);
            print(os,0,now,player,enemy);
        }
        else 
        {
            carry(now,player,enemy);
            carry(Enow,enemy,player);
            print(os,0,now,player,enemy);
            print(os,1,Enow,enemy,player);         
        }
        if(!enemy.alive(enemy.get_UP()))
            os<<"Enemy’s "<<enemy.UP_str()<<" is beaten"<<endl;
        if(!player.alive(player.get_UP()))
            os<<"Your "<<player.UP_str()<<" is beaten"<<endl;
        if(player.lost()||enemy.lost())
        { 
            if(!player.lost())
            {    
                if(!player.alive(player.get_UP()))//场上宠物gg
                {
                    while(true)
                    {
                        os<<"Select your next pet (";
                        os<<player.choices(player.get_UP());
                        os<<"): ";
                        int nxt;
                        is>>nxt;
                        if(nxt!=player.get_UP()&&player.alive(nxt))
                        {
                            player.UP(nxt);
                            os<<"You send "<<player.UP_str()<<endl;
                            break;
                        }
                        else
                            continue;
                    }
                }
                os<<"You Win"<<endl;
            }
            else if(!enemy.lost())
            {
                if(!enemy.alive(enemy.get_UP()))
                {
                    int nowup=player.get_UP();
                    if(enemy.alive((nowup+1)%3+1))
                        enemy.UP((nowup+1)%3+1);
                    else if(enemy.alive(nowup))
                        enemy.UP(nowup);
                    else
                        enemy.UP(nowup%3+1);
                    os<<"Enemy send "<<enemy.UP_str()<<endl;
                }
                os<<"You Lose"<<endl;
            }
            else
                os<<"Draw";
            break;
        }
        if(round==100)
        {
            os<<"Draw";
            break;
        }
        if(!(player.alive(player.get_UP())&&enemy.alive(enemy.get_UP())))
        {
            if(!player.alive(player.get_UP())&&!enemy.alive(enemy.get_UP()))
            {
                int nowup=player.get_UP();
                if(enemy.alive((nowup+1)%3+1))
                    enemy.UP((nowup+1)%3+1);
                else if(enemy.alive(nowup))
                    enemy.UP(nowup);
                else
                    enemy.UP(nowup%3+1);
                while(true)
                {
                    os<<"Select your next pet (";
                    os<<player.choices(player.get_UP());
                    os<<"): ";
                    int nxt;
                    is>>nxt;
                    if(nxt!=player.get_UP()&&player.alive(nxt))
                    {
                        player.UP(nxt);
                        os<<"You send "<<player.UP_str()<<endl;
                        break;
                    }
                    else
                        continue;
                }
                os<<"Enemy send "<<enemy.UP_str()<<endl;        
            }
            else
            {
                if(!enemy.alive(enemy.get_UP()))
                {
                    int nowup=player.get_UP();
                    if(enemy.alive(nowup%3+1))
                        enemy.UP(nowup%3+1);
                    else if(enemy.alive(nowup))
                        enemy.UP(nowup);
                    else
                        enemy.UP((nowup+1)%3+1);
                    os<<"Enemy send "<<enemy.UP_str()<<endl;
                }
                else
                {
                    while(true)
                    {
                        os<<"Select your next pet (";
                        os<<player.choices(player.get_UP());
                        os<<"): ";
                        int nxt;
                        is>>nxt;
                        if(nxt!=player.get_UP()&&player.alive(nxt))
                        {
                            player.UP(nxt);
                            os<<"You send "<<player.UP_str()<<endl;
                            break;
                        }
                        else
                            continue;
                    } 
                }
            }
        }
        os<<"Your "<<player.UP_str()<<": HP "<<player.get_HP()<<" || Enemy’s "<<enemy.UP_str()<<": HP "<<enemy.get_HP()<<endl;
    }
}