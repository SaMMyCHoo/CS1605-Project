#include<cstdio>
#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
using namespace std;
class Pet
{
    public:
    void SetPet(int type)
    {
        used=false;
        TYPE=type;
        SLP_cnt=0;
        if(type==1)//草属性，W
        {
            pet_NAME="W";
            ATK=10,DF=10,max_HP=110,HP=110,SPD=10;
            PW[0]=20,PW[1]=20,PW[2]=0;
            SKL[0]="Tackle";
            SKL[1]="Leaf";
            SKL[2]="Synthesis";
        }
        else if(type==2)//火属性，L
        {
            pet_NAME="L";
            ATK=12,DF=10,max_HP=100,HP=100,SPD=11;
            PW[0]=15,PW[1]=20,PW[2]=0;
            SKL[0]="Claw";
            SKL[1]="Flame";
            SKL[2]="Sunny";
        }
        else if(type==3)//水属性，G
        {
            pet_NAME="G";
            ATK=10,DF=14,max_HP=100,HP=100,SPD=9;
            PW[0]=20,PW[1]=20,PW[2]=0;
            SKL[0]="Tackle";
            SKL[1]="Stream";
            SKL[2]="Rainy";
        }
        else if(type==4)//雷属性，Q
        {
            pet_NAME="Q";
            ATK=10,DF=10,max_HP=90,HP=90,SPD=12;
            PW[0]=15,PW[1]=25,PW[2]=50;
            SKL[0]="Quick";
            SKL[1]="Volt";
            SKL[2]="Thunder";
        }
        else//物理系，D
        {
            pet_NAME="D";
            ATK=9,DF=8,max_HP=120,HP=120,SPD=7;
            PW[0]=15,PW[1]=0,PW[2]=0;
            SKL[0]="Slap";
            SKL[1]="Sing";
            SKL[2]="Paint";
        }
    }
    int get_Type() const
    {
        return TYPE;
    }
    int get_atk() const
    {
        return ATK;
    }
    int get_df() const 
    {
        return DF;
    }
    int get_pw(int t) const
    {
        return PW[t];
    }
    int get_maxhp() const
    {
        return max_HP;
    }
    int get_hp() const
    {
        return HP;
    }
    int get_spd() const
    {
        return SPD;
    }
    string get_name() const
    {
        return pet_NAME;
    }
    string get_skl(int t) const
    {
        return SKL[t];
    }
    bool asleep() const
    {
        return SLP_cnt>0;
    }
    bool painted() const
    {
        return DF<8;
    }
    bool used_now() const
    {
        return used;
    }
    void half_HP()
    {
        HP=max_HP/2;
    }
    void PAN_DF()
    {
        if(!painted())  
            DF/=2;
    }
    void set_slp(bool x)//x=0,清空；x=1,每回合递减
    {
        if(!x)
            SLP_cnt=0;
        else
            SLP_cnt--;
    }
    void add_hp(int hp)
    {
        HP+=hp;
        if(HP<0)
            HP=0;
        if(HP>max_HP)
            HP=max_HP;
    }
    void fall_slp()
    {
        SLP_cnt=3;
    }
    void set_skill(bool m)
    {
        used=m;
    }
    private:
    int max_HP;
    int ATK,DF,HP,PW[3],SPD;//攻击力，防御力，血量，威力，速度
    int TYPE;//属性
    string pet_NAME,SKL[3];//技能名称
    int SLP_cnt;
    bool used;
};
//0普攻，1技能，2辅助
double atk_val(int x,int y,int choice)
{
    if(choice==0)//选择普攻
        return 1.0;
    if(x==5||y==5)//攻/守方是物理属性，全技能倍率均为1
        return 1.0;
    if(x==1&&y==3)
        return 2.0;
    if(x==2&&y==1)
        return 2.0;
    if(x==3&&y==2)
        return 2.0;
    if(x==4&&y==3)
        return 2.0;
    if(x==4&&y==2)
        return 1.0;
    if(x!=4&&y==4)
        return 1.0;
    return 0.5;
}
int atk_dmg(Pet x,Pet y,int choice,int weather)//技能伤害
{
    double damage=1.0*x.get_pw(choice)*x.get_atk()/y.get_df()*atk_val(x.get_Type(),y.get_Type(),choice);
    if(choice==0)//平a
    {
        if(x.get_Type()==5&&y.asleep())//拍打对睡眠生效
            return int(2*damage+0.5);
        return int(damage+0.5);       
    }
    if(weather>0)//晴天
    {
        if(x.get_Type()==2)//火属性
            return int(1.5*damage+0.5);
        if(x.get_Type()==3)//水属性
            return int(0.5*damage+0.5);
    }
    if(weather<0)//雨天
    {
        if(x.get_Type()==3)//水
            return int(1.5*damage+0.5);
        if(x.get_Type()==2)//火
            return int(0.5*damage+0.5);
    }
    return int(damage+0.5);
}
struct Unit
{
    int up;
    Pet pets[3];
    void select(int a,int b,int c)
    {
        up=0;
        pets[0].SetPet(a);
        pets[1].SetPet(b);
        pets[2].SetPet(c);
    }
    int deadcnt() const
    {
        int cnt=0;
        for(int i=0;i<3;++i)
        {
            if(pets[i].get_hp()<=0)
                ++cnt;
        }
        return cnt;
    }
    void change(int p)
    {
        up=p;
    }
}player,enemy;
int weather;
void init(ostream &os,istream &is)
{
    enemy.select(1,3,4);
    enemy.up=rand()%3;
    os<<"Welcome to Battle of Pets 2!"<<endl;
    os<<"Enemy has W, G and Q."<<endl;
    os<<"You can select three from five (1 for W, 2 for L, 3 for G, 4 for Q, 5 for D)."<<endl;
    int tmp[3];
    for(int i=0;i<3;++i)
    {
        while(1)
        {
            os<<"Select your "<<i+1<<"st pet: ";
            is>>tmp[i];
            bool ok=true;
            if(tmp[i]>5||tmp[i]<1)
                ok=false;
            for(int j=0;j<i;++j)
                if(tmp[i]==tmp[j])
                    ok=false;
            if(ok)
                break;
        }
    }
    player.select(tmp[0],tmp[1],tmp[2]);
    os<<"You have "<<player.pets[0].get_name()<<", "<<player.pets[1].get_name()<<" and "<<player.pets[2].get_name()<<"."<<endl;
    os<<"You start with "<<player.pets[0].get_name()<<endl;
    os<<"Enemy starts with "<<enemy.pets[enemy.up].get_name()<<endl;
    os<<"Your "<<player.pets[0].get_name()<<": HP "<<player.pets[0].get_hp();
    os<<" || Enemy’s "<<enemy.pets[enemy.up].get_name()<<": HP "<<enemy.pets[enemy.up].get_hp()<<endl;
    os<<"Battle starts!"<<endl;
}
struct Action
{
    int choice,obj;
    //1技能，2换人，3吃药
}P_c,C_c;
int get_adv(Pet pet,Action act)
{
    if(act.choice==2)
        return 6;
    if(act.choice==3)
        return 5;
    if(pet.get_Type()==4&&act.obj==0)
        return 1;
    if(pet.get_Type()==2&&act.obj==2)
        return 1;
    if(pet.get_Type()==3&&act.obj==2)
        return 1;
    return 0;
}
void excute(ostream &os,istream &is,Unit &x,Unit &y,Action p,Action c,int &weather)
{
    int P_v=get_adv(x.pets[x.up],p);
    int C_v=get_adv(y.pets[y.up],c);
    bool p_fst;
    if(P_v>C_v)
        p_fst=true;
    else if(P_v<C_v)
        p_fst=false;
    else
    {
        if(x.pets[x.up].get_spd()>y.pets[y.up].get_spd())
            p_fst=true;
        else 
            p_fst=false;
    }
    //执行操作
    if(p_fst)//玩家宠物先出手
    {
        if(p.choice==1)//进攻
        {
            if(x.pets[x.up].asleep())//睡着了，欸嘿
            {
                os<<"Your "<<x.pets[x.up].get_name()<<" is sleeping"<<endl;
            }
            else
            {
                if(p.obj==0)//平a
                {
                    os<<"Your "<<x.pets[x.up].get_name()<<" uses "<<x.pets[x.up].get_skl(0)<<"! ";
                    int tmp=atk_dmg(x.pets[x.up],y.pets[y.up],0,weather);
                    os<<"Damage: "<<tmp<<endl;
                    x.pets[x.up].set_skill(true);
                    y.pets[y.up].add_hp(-tmp);
                    if(y.pets[y.up].get_hp()<=0)
                        y.pets[y.up].set_slp(false);
                    else if(y.pets[y.up].asleep())
                    {
                        y.pets[y.up].set_slp(false);
                        os<<"Enemy's "<<y.pets[y.up].get_name()<<" wakes up"<<endl;
                    }
                }
                else if(p.obj==1)//技能1
                {
                    if(x.pets[x.up].get_Type()!=5)
                    {
                        os<<"Your "<<x.pets[x.up].get_name()<<" uses "<<x.pets[x.up].get_skl(1)<<"! ";
                        int tmp=atk_dmg(x.pets[x.up],y.pets[y.up],1,weather);
                        os<<"Damage: "<<tmp<<endl;
                        y.pets[y.up].add_hp(-tmp);
                        x.pets[x.up].set_skill(true);
                        if(y.pets[y.up].get_hp()<=0)
                            y.pets[y.up].set_slp(false);
                        else if(y.pets[y.up].asleep())
                        {
                            y.pets[y.up].set_slp(false);
                            os<<"Enemy's "<<y.pets[y.up].get_name()<<" wakes up"<<endl;
                        }
                    }
                    else//唱歌
                    {
                        os<<"Your "<<x.pets[x.up].get_name()<<" uses "<<x.pets[x.up].get_skl(1)<<"! ";
                        if(!y.pets[y.up].asleep())
                        {
                            os<<"Enemy's "<<y.pets[y.up].get_name()<<" falls asleep"<<endl;
                            y.pets[y.up].fall_slp();
                        }
                        else
                            os<<"Waste"<<endl;
                        x.pets[x.up].set_skill(true);
                    }
                }
                else//技能2
                {
                    if(x.pets[x.up].get_Type()==1)//光合
                    {
                        x.pets[x.up].set_skill(true);
                        double Hp;
                        if(weather>0)//40%
                            Hp=0.4;
                        else if(weather<0)
                            Hp=0.2;
                        else
                            Hp=0.3;
                        int val=int(0.5+x.pets[x.up].get_maxhp()*Hp);
                        os<<"Your W uses Synthesis! Recovery: "<<val<<endl;
                        x.pets[x.up].add_hp(val);
                    }
                    else if(x.pets[x.up].get_Type()==2)//放晴
                    {
                        x.pets[x.up].set_skill(true);
                        os<<"Your L uses Sunny! Weather becomes sunny"<<endl;
                        weather=5;
                    }
                    else if(x.pets[x.up].get_Type()==3)//下雨
                    {
                        x.pets[x.up].set_skill(true);
                        os<<"Your G uses Rainy! Weather becomes rainy"<<endl;
                        weather=-5;
                    }
                    else if(x.pets[x.up].get_Type()==4)//打雷
                    {
                        os<<"Your Q uses Thunder! ";
                        if(weather<0)
                        {
                            x.pets[x.up].set_skill(true);
                            int tmp=atk_dmg(x.pets[x.up],y.pets[y.up],2,weather);
                            os<<"Damage: "<<tmp<<endl;
                            y.pets[y.up].add_hp(-tmp);
                            if(y.pets[y.up].get_hp()<=0)
                                y.pets[y.up].set_slp(false);
                            else if(y.pets[y.up].asleep())
                            {
                                y.pets[y.up].set_slp(false);
                                os<<"Enemy's "<<y.pets[y.up].get_name()<<" wakes up"<<endl;
                            }
                        }
                        else
                            os<<"Failure"<<endl;
                    }
                    else//涂鸦
                    {   
                        os<<"Your D uses Paint! ";
                        if(!y.pets[y.up].used_now())
                        {
                            x.pets[x.up].set_skill(true);
                            os<<"Enemy's "<<y.pets[y.up].get_name()<<" is painted"<<endl;
                            y.pets[y.up].PAN_DF();
                        }
                        else
                            os<<"Failure"<<endl;
                    }
                }
            }
        }
        else//换人
        {
            x.change(p.obj);
            os<<"You send "<<x.pets[x.up].get_name()<<endl;
        }
        //敌方操作
        //首先如果已经被打死了就要换人！
        //换人后无法操作
        if(y.pets[y.up].get_hp()<=0)
        {
            os<<"Enemy's "<<y.pets[y.up].get_name()<<" is beaten"<<endl;
            int nxt;
            if(y.deadcnt()==3)
                return;
            if(y.deadcnt()==1)
            {
                if(y.up!=1)//W或者Q死了，上G
                    nxt=1;
                else//G死了，上W
                    nxt=0;
            }
            else if(y.deadcnt()==2)
            {
                //有谁上谁就好，问题不大
                for(int i=0;i<3;++i)
                {
                    if(y.pets[i].get_hp()>0)
                    {
                        nxt=i;
                        break;
                    }
                }
            }
            y.up=nxt;
            os<<"Enemy sends "<<y.pets[y.up].get_name()<<endl;
        }
        else
        {
            if(c.choice==1)//进攻
            {
                if(y.pets[y.up].asleep())//睡着了，欸嘿
                {
                    os<<"Enemy's "<<y.pets[y.up].get_name()<<" is sleeping"<<endl;
                }
                else
                {
                    if(c.obj==0)//平a
                    {
                        os<<"Enemy's "<<y.pets[y.up].get_name()<<" uses "<<y.pets[y.up].get_skl(0)<<"! ";
                        int tmp=atk_dmg(y.pets[y.up],x.pets[x.up],0,weather);
                        os<<"Damage: "<<tmp<<endl;
                        y.pets[y.up].set_skill(true);
                        x.pets[x.up].add_hp(-tmp);
                        if(x.pets[x.up].get_hp()<=0)
                            x.pets[x.up].set_slp(false);
                        else if(x.pets[x.up].asleep())
                        {
                            x.pets[x.up].set_slp(false);
                            os<<"Your "<<x.pets[x.up].get_name()<<" wakes up"<<endl;
                        }
                    }
                    else if(c.obj==1)//技能1
                    {
                        if(y.pets[y.up].get_Type()!=5)
                        {
                            os<<"Enemy's "<<y.pets[y.up].get_name()<<" uses "<<y.pets[y.up].get_skl(1)<<"! ";
                            int tmp=atk_dmg(y.pets[y.up],x.pets[x.up],1,weather);
                            os<<"Damage: "<<tmp<<endl;
                            x.pets[x.up].add_hp(-tmp);
                            y.pets[y.up].set_skill(true);
                            if(x.pets[x.up].get_hp()<=0)
                                x.pets[x.up].set_slp(false);
                            else if(x.pets[x.up].asleep())
                            {
                                x.pets[x.up].set_slp(false);
                                os<<"Your "<<x.pets[x.up].get_name()<<" wakes up"<<endl;
                            }
                        }
                        else//唱歌
                        {
                            os<<"Enemy's "<<y.pets[y.up].get_name()<<" uses "<<y.pets[y.up].get_skl(1)<<"! ";
                            if(!x.pets[x.up].asleep())
                            {
                                os<<"Your "<<x.pets[x.up].get_name()<<" falls asleep"<<endl;
                                x.pets[x.up].fall_slp();
                            }
                            else
                                os<<"Waste"<<endl;
                            y.pets[y.up].set_skill(true);
                        }
                    }
                    else//技能2
                    {
                        if(y.pets[y.up].get_Type()==1)//光合
                        {
                            y.pets[y.up].set_skill(true);
                            double Hp;
                            if(weather>0)//40%
                                Hp=0.4;
                            else if(weather<0)
                                Hp=0.2;
                            else
                                Hp=0.3;
                            int val=int(0.5+y.pets[y.up].get_maxhp()*Hp);
                            os<<"Enemy's W uses Synthesis! Recovery: "<<val<<endl;
                            y.pets[y.up].add_hp(val);
                        }
                        else if(y.pets[y.up].get_Type()==2)//放晴
                        {
                            y.pets[y.up].set_skill(true);
                            os<<"Enemy's L uses Sunny! Weather becomes sunny"<<endl;
                            weather=5;
                        }
                        else if(y.pets[y.up].get_Type()==3)//下雨
                        {
                            y.pets[y.up].set_skill(true);
                            os<<"Enemy's G uses Rainy! Weather becomes rainy"<<endl;
                            weather=-5;
                        }
                        else if(y.pets[y.up].get_Type()==4)//打雷
                        {
                            os<<"Enemy's Q uses Thunder! ";
                            if(weather<0)
                            {
                                y.pets[y.up].set_skill(true);
                                int tmp=atk_dmg(y.pets[y.up],x.pets[x.up],2,weather);
                                os<<"Damage: "<<tmp<<endl;
                                x.pets[x.up].add_hp(-tmp);
                                if(x.pets[x.up].get_hp()<=0)
                                    x.pets[x.up].set_slp(false);
                                else if(x.pets[x.up].asleep())
                                {
                                    x.pets[x.up].set_slp(false);
                                    os<<"Your "<<x.pets[x.up].get_name()<<" wakes up"<<endl;
                                }
                            }
                            else
                                os<<"Failure"<<endl;
                        }
                        else//涂鸦
                        {   
                            os<<"Enemy's D uses Paint! ";
                            if(!x.pets[x.up].used_now())
                            {
                                y.pets[y.up].set_skill(true);
                                os<<"Your "<<x.pets[x.up].get_name()<<" is painted"<<endl;
                                x.pets[x.up].PAN_DF();
                            }
                            else
                                os<<"Failure"<<endl;
                        }
                    }
                }
            }
            else if(c.choice==2)//换人
            {
                y.change(c.obj);
                os<<"Enemy send "<<y.pets[y.up].get_name()<<endl;              
            }
            else
            {
                os<<"Enemy uses Revival Potion on G"<<endl;
                y.pets[1].half_HP();
            }
        }
        if(x.pets[x.up].get_hp()<=0)
        {
            os<<"Your "<<x.pets[x.up].get_name()<<" is beaten"<<endl;
            if(x.deadcnt()==3)
                return;
            while(1)
            {
                os<<"Select your next pet (";
                if(x.deadcnt()==2)
                {
                    for(int i=0;i<3;++i)
                    {
                        if(x.pets[i].get_hp()>0)
                        {
                            os<<i+1<<" for "<<x.pets[i].get_name()<<"-HP"<<player.pets[i].get_hp()<<"): ";
                            break;
                        }
                    }
                }
                else
                {
                    if(x.pets[0].get_hp()<=0)
                    {
                        os<<"2 for "<<x.pets[1].get_name()<<"-HP"<<player.pets[1].get_hp();
                        os<<", 3 for "<<x.pets[2].get_name()<<"-HP"<<player.pets[2].get_hp()<<"): ";
                    }
                    else if(x.pets[1].get_hp()<=0)
                    {
                        os<<"1 for "<<x.pets[0].get_name()<<"-HP"<<player.pets[0].get_hp();
                        os<<", 3 for "<<x.pets[2].get_name()<<"-HP"<<player.pets[2].get_hp()<<"): ";
                    }                    
                    else if(x.pets[2].get_hp()<=0)
                    {
                        os<<"1 for "<<x.pets[0].get_name()<<"-HP"<<player.pets[0].get_hp();
                        os<<", 2 for "<<x.pets[1].get_name()<<"-HP"<<player.pets[1].get_hp()<<"): ";
                    }
                }
                int now;
                is>>now;
                if(x.pets[now-1].get_hp()<=0)
                    continue;
                else
                {
                    x.up=now-1;
                    os<<"You send "<<x.pets[x.up].get_name()<<endl;
                    break;
                }
            }
        }
    }
    else
    {
        if(c.choice==1)//进攻
        {
            if(y.pets[y.up].asleep())//睡着了，欸嘿
            {
                os<<"Enemy's "<<y.pets[y.up].get_name()<<" is sleeping"<<endl;
            }
            else
            {
                if(c.obj==0)//平a
                {
                    os<<"Enemy's "<<y.pets[y.up].get_name()<<" uses "<<y.pets[y.up].get_skl(0)<<"! ";
                    int tmp=atk_dmg(y.pets[y.up],x.pets[x.up],0,weather);
                    os<<"Damage: "<<tmp<<endl;
                    y.pets[y.up].set_skill(true);
                    x.pets[x.up].add_hp(-tmp);
                    if(x.pets[x.up].get_hp()<=0)
                        x.pets[x.up].set_slp(false);
                    else if(x.pets[x.up].asleep())
                    {
                        x.pets[x.up].set_slp(false);
                        os<<"Your "<<x.pets[x.up].get_name()<<" wakes up"<<endl;
                    }
                }
                else if(c.obj==1)//技能1
                {
                    if(y.pets[y.up].get_Type()!=5)
                    {
                        os<<"Enemy's "<<y.pets[y.up].get_name()<<" uses "<<y.pets[y.up].get_skl(1)<<"! ";
                        int tmp=atk_dmg(y.pets[y.up],x.pets[x.up],1,weather);
                        os<<"Damage: "<<tmp<<endl;
                        x.pets[x.up].add_hp(-tmp);
                        y.pets[y.up].set_skill(true);
                        if(x.pets[x.up].get_hp()<=0)
                            x.pets[x.up].set_slp(false);
                        else if(x.pets[x.up].asleep())
                        {
                            x.pets[x.up].set_slp(false);
                            os<<"Your "<<x.pets[x.up].get_name()<<" wakes up"<<endl;
                        }
                    }
                    else//唱歌
                    {
                        os<<"Enemy's "<<y.pets[y.up].get_name()<<" uses "<<y.pets[y.up].get_skl(1)<<"! ";
                        if(!x.pets[x.up].asleep())
                        {
                            os<<"Your "<<x.pets[x.up].get_name()<<" falls asleep"<<endl;
                            x.pets[x.up].fall_slp();
                        }
                        else
                            os<<"Waste"<<endl;
                        y.pets[y.up].set_skill(true);
                    }
                }
                else//技能2
                {
                    if(y.pets[y.up].get_Type()==1)//光合
                    {
                        y.pets[y.up].set_skill(true);
                        double Hp;
                        if(weather>0)//40%
                            Hp=0.4;
                        else if(weather<0)
                            Hp=0.2;
                        else
                            Hp=0.3;
                        int val=int(0.5+y.pets[y.up].get_maxhp()*Hp);
                        os<<"Enemy's W uses Synthesis! Recovery: "<<val<<endl;
                        y.pets[y.up].add_hp(val);
                    }
                    else if(y.pets[y.up].get_Type()==2)//放晴
                    {
                        y.pets[y.up].set_skill(true);
                        os<<"Enemy's L uses Sunny! Weather becomes sunny"<<endl;
                        weather=5;
                    }
                    else if(y.pets[y.up].get_Type()==3)//下雨
                    {
                        y.pets[y.up].set_skill(true);
                        os<<"Enemy's G uses Rainy! Weather becomes rainy"<<endl;
                        weather=-5;
                    }
                    else if(y.pets[y.up].get_Type()==4)//打雷
                    {
                        os<<"Enemy's Q uses Thunder! ";
                        if(weather<0)
                        {
                            y.pets[y.up].set_skill(true);
                            int tmp=atk_dmg(y.pets[y.up],x.pets[x.up],2,weather);
                            os<<"Damage: "<<tmp<<endl;
                            x.pets[x.up].add_hp(-tmp);
                            if(x.pets[x.up].get_hp()<=0)
                                x.pets[x.up].set_slp(false);
                            else if(x.pets[x.up].asleep())
                            {
                                x.pets[x.up].set_slp(false);
                                os<<"Your "<<x.pets[x.up].get_name()<<" wakes up"<<endl;
                            }
                        }
                        else
                            os<<"Failure"<<endl;
                    }
                    else//涂鸦
                    {   
                        os<<"Enemy's D uses Paint! ";
                        if(!x.pets[x.up].used_now())
                        {
                            y.pets[y.up].set_skill(true);
                            os<<"Your "<<x.pets[x.up].get_name()<<" is painted"<<endl;
                            x.pets[x.up].PAN_DF();
                        }
                        else
                            os<<"Failure"<<endl;
                    }
                }
            }
        }
        else if(c.choice==2)//换人
        {
            y.change(c.obj);
            os<<"Enemy send "<<y.pets[y.up].get_name()<<endl;              
        }
        else
        {
            os<<"Enemy uses Revival Potion on G"<<endl;
            y.pets[1].half_HP();
        }
        if(x.pets[x.up].get_hp()<=0)
        {
            os<<"Your "<<x.pets[x.up].get_name()<<" is beaten"<<endl;
            if(x.deadcnt()==3)
                return;
            while(1)
            {
                os<<"Select your next pet (";
                if(x.deadcnt()==2)
                {
                    for(int i=0;i<3;++i)
                    {
                        if(x.pets[i].get_hp()>0)
                        {
                            os<<i+1<<" for "<<x.pets[i].get_name()<<"-HP"<<player.pets[i].get_hp()<<"): ";
                            break;
                        }
                    }
                }
                else
                {
                    if(x.pets[0].get_hp()<=0)
                    {
                        os<<"2 for "<<x.pets[1].get_name()<<"-HP"<<player.pets[1].get_hp();
                        os<<", 3 for "<<x.pets[2].get_name()<<"-HP"<<player.pets[2].get_hp()<<"): ";
                    }
                    else if(x.pets[1].get_hp()<=0)
                    {
                        os<<"1 for "<<x.pets[0].get_name()<<"-HP"<<player.pets[0].get_hp();
                        os<<", 3 for "<<x.pets[2].get_name()<<"-HP"<<player.pets[2].get_hp()<<"): ";
                    }                    
                    else if(x.pets[2].get_hp()<=0)
                    {
                        os<<"1 for "<<x.pets[0].get_name()<<"-HP"<<player.pets[0].get_hp();
                        os<<", 2 for "<<x.pets[1].get_name()<<"-HP"<<player.pets[1].get_hp()<<"): ";
                    }
                }
                int now;
                is>>now;
                if(x.pets[now-1].get_hp()<=0)
                    continue;
                else
                {
                    x.up=now-1;
                    os<<"You send "<<x.pets[x.up].get_name()<<endl;
                    break;
                }
            }
        }
        else
        {
            if(p.choice==1)//进攻
            {
                if(x.pets[x.up].asleep())//睡着了，欸嘿
                {
                    os<<"Your "<<x.pets[x.up].get_name()<<" is sleeping"<<endl;
                }
                else
                {
                    if(p.obj==0)//平a
                    {
                        os<<"Your "<<x.pets[x.up].get_name()<<" uses "<<x.pets[x.up].get_skl(0)<<"! ";
                        int tmp=atk_dmg(x.pets[x.up],y.pets[y.up],0,weather);
                        os<<"Damage: "<<tmp<<endl;
                        x.pets[x.up].set_skill(true);
                        y.pets[y.up].add_hp(-tmp);
                        if(y.pets[y.up].get_hp()<=0)
                            y.pets[y.up].set_slp(false);
                        else if(y.pets[y.up].asleep())
                        {
                            y.pets[y.up].set_slp(false);
                            os<<"Enemy's "<<y.pets[y.up].get_name()<<" wakes up"<<endl;
                        }
                    }
                    else if(p.obj==1)//技能1
                    {
                        if(x.pets[x.up].get_Type()!=5)
                        {
                            os<<"Your "<<x.pets[x.up].get_name()<<" uses "<<x.pets[x.up].get_skl(1)<<"! ";
                            int tmp=atk_dmg(x.pets[x.up],y.pets[y.up],1,weather);
                            os<<"Damage: "<<tmp<<endl;
                            y.pets[y.up].add_hp(-tmp);
                            x.pets[x.up].set_skill(true);
                            if(y.pets[y.up].get_hp()<=0)
                                y.pets[y.up].set_slp(false);
                            else if(y.pets[y.up].asleep())
                            {
                                y.pets[y.up].set_slp(false);
                                os<<"Enemy's "<<y.pets[y.up].get_name()<<" wakes up"<<endl;
                            }
                        }
                        else//唱歌
                        {
                            os<<"Your "<<x.pets[x.up].get_name()<<" uses "<<x.pets[x.up].get_skl(1)<<"! ";
                            if(!y.pets[y.up].asleep())
                            {
                                os<<"Enemy's "<<y.pets[y.up].get_name()<<" falls asleep"<<endl;
                                y.pets[y.up].fall_slp();
                            }
                            else
                                os<<"Waste"<<endl;
                            x.pets[x.up].set_skill(true);
                        }
                    }
                    else//技能2
                    {
                        if(x.pets[x.up].get_Type()==1)//光合
                        {
                            x.pets[x.up].set_skill(true);
                            double Hp;
                            if(weather>0)//40%
                                Hp=0.4;
                            else if(weather<0)
                                Hp=0.2;
                            else
                                Hp=0.3;
                            int val=int(0.5+x.pets[x.up].get_maxhp()*Hp);
                            os<<"Your W uses Synthesis! Recovery: "<<val<<endl;
                            x.pets[x.up].add_hp(val);
                        }
                        else if(x.pets[x.up].get_Type()==2)//放晴
                        {
                            x.pets[x.up].set_skill(true);
                            os<<"Your L uses Sunny! Weather becomes sunny"<<endl;
                            weather=5;
                        }
                        else if(x.pets[x.up].get_Type()==3)//下雨
                        {
                            x.pets[x.up].set_skill(true);
                            os<<"Your G uses Rainy! Weather becomes rainy"<<endl;
                            weather=-5;
                        }
                        else if(x.pets[x.up].get_Type()==4)//打雷
                        {
                            os<<"Your Q uses Thunder! ";
                            if(weather<0)
                            {
                                x.pets[x.up].set_skill(true);
                                int tmp=atk_dmg(x.pets[x.up],y.pets[y.up],2,weather);
                                os<<"Damage: "<<tmp<<endl;
                                y.pets[y.up].add_hp(-tmp);
                                if(y.pets[y.up].get_hp()<=0)
                                    y.pets[y.up].set_slp(false);
                                else if(y.pets[y.up].asleep())
                                {
                                    y.pets[y.up].set_slp(false);
                                    os<<"Enemy's "<<y.pets[y.up].get_name()<<" wakes up"<<endl;
                                }
                            }
                            else
                                os<<"Failure"<<endl;
                        }
                        else//涂鸦
                        {   
                            os<<"Your D uses Paint! ";
                            if(!y.pets[y.up].used_now())
                            {
                                x.pets[x.up].set_skill(true);
                                os<<"Enemy's "<<y.pets[y.up].get_name()<<" is painted"<<endl;
                                y.pets[y.up].PAN_DF();
                            }
                            else
                                os<<"Failure"<<endl;
                        }
                    }
                }
            }
            else//换人
            {
                x.change(p.obj);
                os<<"You send "<<x.pets[x.up].get_name()<<endl;
            }
            if(y.pets[y.up].get_hp()<=0)
            {
                os<<"Enemy's "<<y.pets[y.up].get_name()<<" is beaten"<<endl;
                int nxt;
                if(y.deadcnt()==3)
                    return;
                if(y.deadcnt()==1)
                {
                    if(y.up!=1)//W或者Q死了，上G
                        nxt=1;
                    else//G死了，上W
                        nxt=0;
                }
                else if(y.deadcnt()==2)
                {
                    //有谁上谁就好，问题不大
                    for(int i=0;i<3;++i)
                    {
                        if(y.pets[i].get_hp()>0)
                        {
                            nxt=i;
                            break;
                        }
                    }
                }
                y.up=nxt;
                os<<"Enemy sends "<<y.pets[y.up].get_name()<<endl;
            }                       
        }           
    }  
}
void play(ostream &os,istream &is)
{
    bool drug=true;
    bool p_wake=false,e_wake=false;
    int round=1;
    int weather=0;//天气为负数则是雨天，正数是晴天，绝对值代表剩余天数
    while(1)
    {
        os<<"--------------------------------------------------"<<endl;
        if(weather>0)
        {
            --weather;
            if(!weather)
                os<<"Weather becomes normal"<<endl;
        }
        else if(weather<0)
        {
            ++weather;        
            if(!weather)
                os<<"Weather becomes normal"<<endl;
        }
        if(e_wake)
            os<<"Enemy's "<<enemy.pets[enemy.up].get_name()<<" wakes up"<<endl;
        if(p_wake)
            os<<"Your "<<player.pets[player.up].get_name()<<" wakes up"<<endl;
        e_wake=false;
        p_wake=false;
        for(int i=0;i<3;++i)
        {
            player.pets[i].set_skill(false);
            enemy.pets[i].set_skill(false);
        }
        os<<"Round "<<round++<<endl;
        while(1)
        {
            os<<"Select your action (0 for escape, 1 for skill";
            if(player.deadcnt()<2)
                os<<", 2 for change";
            os<<"): ";
            int tmp;
            is>>tmp;
            if(tmp==0)
            {
                P_c.choice=0;
                os<<"You Escape"<<endl;
                return;
            }
            else if(tmp==1)
            {
                P_c.choice=1;
                bool back=false;
                while(1)
                {
                    os<<"Select the skill (0 for back, ";
                    os<<"1 for "<<player.pets[player.up].get_skl(0)<<", 2 for ";
                    os<<player.pets[player.up].get_skl(1)<<", 3 for "<<player.pets[player.up].get_skl(2)<<"): ";
                    int now;
                    is>>now;
                    if(now==0)
                    {
                        back=true;
                        break;
                    }
                    else if(now==1||now==2||now==3)
                    {
                        P_c.obj=now-1;
                        break;
                    }
                    else
                        continue;
                }
                if(back)
                    continue;
                else
                    break;
            }
            else if(tmp==2)
            {
                if(player.deadcnt()<2)
                {
                    P_c.choice=2;
                    bool back=false;
                    while(1)
                    {
                        os<<"Select your next pet (0 for back";
                        if(player.up!=0&&player.pets[0].get_hp()>0)
                            os<<", 1 for "<<player.pets[0].get_name()<<"-HP"<<player.pets[0].get_hp();
                        if(player.up!=1&&player.pets[1].get_hp()>0)
                            os<<", 2 for "<<player.pets[1].get_name()<<"-HP"<<player.pets[1].get_hp();
                        if(player.up!=2&&player.pets[2].get_hp()>0)
                            os<<", 3 for "<<player.pets[2].get_name()<<"-HP"<<player.pets[2].get_hp();
                        os<<"): ";
                        int now;
                        is>>now;
                        if(now==0)
                        {
                            back=true;
                            break;
                        }
                        else
                        {
                            if(now!=1&&now!=2&&now!=3)
                                continue;
                            if(now-1==player.up)
                                continue;
                            if(player.pets[now-1].get_hp()<=0)
                                continue;
                            P_c.obj=now-1;
                            break;
                        }
                    }
                    if(back)
                        continue;
                    else
                        break;
                }
                else
                    continue;
            }
            else
                continue;
        }
        if(enemy.up==0)//W
        {
            if(enemy.pets[1].get_hp()<=0&&drug)
            {
                drug=false;
                C_c.choice=3,C_c.obj=1;
            }
            else
            {
                if(player.pets[player.up].get_Type()==2)//对方是火属性
                {
                    if(enemy.pets[1].get_hp()>0)
                    {
                        C_c.choice=2,C_c.obj=1;//换成小龟
                    }
                    else
                    {
                        if(enemy.pets[0].get_hp()*3>=enemy.pets[0].get_maxhp()*2)
                            C_c.choice=1,C_c.obj=0;
                        else
                            C_c.choice=1,C_c.obj=2;
                    }
                }
                else if(player.pets[player.up].get_Type()==1)
                {
                    if(enemy.pets[0].get_hp()*2>=enemy.pets[0].get_maxhp())
                        C_c.choice=1,C_c.obj=0;
                    else
                        C_c.choice=1,C_c.obj=2;                       
                }
                else
                {
                    if(enemy.pets[0].get_hp()*2>=enemy.pets[0].get_maxhp())
                        C_c.choice=1,C_c.obj=1;
                    else if(player.pets[player.up].get_hp()*5<player.pets[player.up].get_maxhp())
                        C_c.choice=1,C_c.obj=1;
                    else
                        C_c.choice=1,C_c.obj=2;
                }
            }
        }
        else if(enemy.up==1)//G
        {
            if(weather>0)
            {
                C_c.choice=1,C_c.obj=2;//下雨
            }
            else if((player.pets[player.up].get_Type()==4)||(player.pets[player.up].get_Type()==1&&weather<0))
            {
                if(enemy.pets[0].get_hp()>0)//W存活
                    C_c.choice=2,C_c.obj=0;
                else
                {
                    if(weather<0&&enemy.pets[2].get_hp()>0)
                        C_c.choice=2,C_c.obj=2;
                    else
                        C_c.choice=1,C_c.obj=1;
                }
            }
            else if(player.pets[player.up].get_Type()==3)
                C_c.choice=1,C_c.obj=0;
            else
            {
                if(weather<0)
                    C_c.choice=1,C_c.obj=1;
                else
                    C_c.choice=1,C_c.obj=2;
            }
        }
        else//Q
        {
            if(enemy.pets[1].get_hp()<=0&&drug)
            {
                drug=false;
                C_c.choice=3,C_c.obj=1;
            }
            else
            {
                if(player.pets[player.up].get_hp()*5<player.pets[player.up].get_maxhp())
                    C_c.choice=1,C_c.obj=0;
                else
                {
                    if(player.pets[player.up].get_Type()==2)
                    {
                        if(enemy.pets[1].get_hp()>0)
                            C_c.choice=2,C_c.obj=1;
                        else
                            C_c.choice=1,C_c.obj=1;
                    }
                    else
                    {
                        if(weather<0)
                            C_c.choice=1,C_c.obj=2;
                        else
                        {
                            if(enemy.pets[0].get_hp()>0)
                                C_c.choice=2,C_c.obj=0;
                            else
                            {
                                if(player.pets[player.up].get_Type()==1||player.pets[player.up].get_Type()==4)
                                    C_c.choice=1,C_c.obj=0;
                                else
                                    C_c.choice=1,C_c.obj=1;
                            }
                        }
                    }
                }
            }           
        }
        excute(os,is,player,enemy,P_c,C_c,weather);
        if(player.deadcnt()==3)
        {
            os<<"You lose"<<endl;
            return;
        }
        if(enemy.deadcnt()==3)
        {
            os<<"You win"<<endl;
            return;
        }
        if(round==100)
        {
            os<<"Draw"<<endl;
            return;
        }
        os<<"Your "<<player.pets[player.up].get_name()<<": HP "<<player.pets[player.up].get_hp();
        os<<" || Enemy’s "<<enemy.pets[enemy.up].get_name()<<": HP "<<enemy.pets[enemy.up].get_hp()<<endl;
        if(enemy.pets[enemy.up].asleep())
        {
            enemy.pets[enemy.up].set_slp(true);
            if(!enemy.pets[enemy.up].asleep())
                e_wake=true;
        }
        if(player.pets[player.up].asleep())
        {
            player.pets[player.up].set_slp(true);
            if(!player.pets[player.up].asleep())
                p_wake=true;
        }
    }
}
int main()
{
    srand(time(0));
    init(cout,cin);
    play(cout,cin);
    return 0;
}
