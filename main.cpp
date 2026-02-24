
#include <chrono>
#include <cstdint>
#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>


uint64_t timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

//#include <windows.h>
#include <GL/glut.h>
#include <fstream>
#include <vector>
#include <map>
#include "shapes.h"
#include <string>
#include <math.h>
#include <iomanip>


float speed_per_second=30,jump_height=5;
bool move_cube=0;
bool game_start=0;

//in form (time in ms,animation,direction)
ROAD road[]= {{1486,0,1},{2828,0,0},{3277,0,1},{4597,3,0},{5030,3,1},{5968,3,0},{6418,3,3},{6862,3,2},{7295,3,3},{7736,3,2}//10
    ,{8185,3,3},{8651,3,2},{9978,0,3},{10396,2,0},{11703,2,3},{12176,1,0},{13054,2,3},{13491,1,2},{13920,1,3},{14387,1,2}
    ,{14816,1,3},{15281,1,2},{15718,1,3},{16609,0,2},{17063,1,1},{17517,1,2},{18820,1,1},{19251,1,2},{20181,1,3},{20619,1,2}
    ,{21050,1,3},{21500,1,2},{21963,1,3},{22415,1,2},{22857,1,3},{23742,1,0},{24199,0,3},{24676,0,2},{24946,1,3},{25937,0,2}
    ,{26389,1,3},{26825,2,2},{27267,1,3},{27704,2,2},{28151,1,3},{28604,2,2},{29053,1,3},{29506,1,2},{29965,2,1},{30840,0,2}
    ,{31267,0,1},{31726,3,2},{33090,0,1},{33526,3,2},{34409,0,3},{34878,1,2},{35330,0,1},{35750,0,2},{36198,0,1},{36634,0,2}

    ,{37091,0,1},{37366,2,2},{37625,0,1},{37914,0,2},{38811,3,3},{39700,1,2},{40034,0,3},{40308,0,2},{40608,0,1},{41501,0,2}
    ,{42392,2,3},{43276,0,2},{44164,0,1},{44618,1,2},{45935,0,1},{46406,1,2},{47293,0,1},{47751,0,0},{48217,0,1},{49535,0,0}
    ,{49971,0,1},{50393,0,0},{50847,0,1},{51285,0,0},{51739,2,1},{53086,1,0},{53543,0,3},{54407,0,0},{54878,0,1},{55160,0,0}
    ,{56561,1,1},{57058,0,2},{57554,2,1},{58005,2,0},{58449,0,1},{58897,1,0},{60245,0,1},{60675,0,0},{61563,0,1},{61982,0,0}
    ,{62422,0,1},{63713,1,0},{64186,0,1},{64653,0,0},{65064,0,1},{65536,1,0},{65984,0,1},{67291,1,0},{67746,0,1},{68611,0,2}

    ,{69094,0,1},{69369,0,2},{70807,3,1},{71268,0,0},{71729,0,1},{72202,1,0},{72634,0,1},{75543,0,0},{75861,0,3},{76180,0,0}
    ,{77484,0,3},{77949,0,0},{79131,1,1},{79431,0,0},{79741,0,1},{81064,0,0},{81540,0,1},{82183,0,0},{82392,0,1},{82706,0,0}

    ,{83011,0,1},{83275,0,0},{84176,0,1},{84628,0,2},{85112,0,1},{85391,0,2},{86853,0,1},{87298,1,0},{87727,1,1},{88179,1,0}
    ,{88607,1,1},{89057,1,0},{89500,1,1},{89940,1,0},{90388,1,1},{91692,1,0},{92181,0,1},{93300,0,0},{93604,0,1},{93918,0,2}
    ,{95315,0,1},{95748,0,2},{96384,0,1},{96601,0,0},{96914,2,1},{97222,2,2},{97501,0,1},{97961,0,2},{98405,2,1},{98860,2,0}
    ,{99306,2,1},{99752,2,0},{100203,2,3},{100643,2,0},{101082,2,1},{101542,2,0},{101954,2,1},{102398,2,2},{102809,2,1},{103267,2,0}
    ,{103723,2,1},{104165,2,0},{104601,2,3},{105038,2,0},{105492,0,1},{105948,1,0},{106411,0,1},{106670,0,0},{107279,0,1},{107726,0,0}
    ,{108179,0,1},{108627,0,0},{109083,0,1},{109554,0,2},{109986,0,1},{110241,0,2},{110883,0,1},{111318,0,2},{111744,0,1},{112185,0,0}
    ,{112601,0,1},{113062,0,0},{113534,0,1},{113821,0,0},{114390,0,1},{114855,0,0},{115295,0,3},{115727,1,0},{116150,1,1},{116603,1,0}
    ,{117047,1,3},{117344,2,0},{118442,2,3},{118889,2,0},{119349,2,3},{125378,0,0}
};







const int LEFT=0,UP=1,RIGHT=2,DOWN=3;// up (1,0,0)
const int ROAD_MAKING_MODE=-2,EDIT_MODE=-1,NORMAL=0,FALL=1,NORMAL_JUMP=2,HALF_JUMP=3,TELEPORT=4/*not implemented*/,ELEVATOR=5;

vector<point> mainPoint,texPoint;//points are stored here
vector<poly> mainPoly;//polygones stored here
vector<object> mainObject;//objects stored here
vector<color> mainColor;//colors ...
map<string,int> nameToColor;
vector<animation> animationStack;

vect position= {-13.7234, -11.4469,  17.6397},up,cen;
float angle1=0.8,angle2=-0.8 ;
float x=0,y=0,z=0,sz;
point last_position= {0,0,0};
float animation_start_height=0;
int situation=0,last_road=0;
float GRAVITY=0.02;
int zic_zac_diraction[2]= {1,0};
int dir=zic_zac_diraction[0];
point line_road_last[4]= {{-1,-1,0},{-1,+1,0},{-1,-1,2},{-1,+1,2}};
uint64_t start,endt,move_start,move_end,animation_start_time;
vector<uint64_t> times;
float speed;

bool on_the_road(float x,float y,float z,float road_size=3)
{
    int b=1;
    float getx=0,gety=0;
    int past=0,k=0;
    for(ROAD r:road)
    {
        b=r.direction;
        if(b==0)
        {
            if((r.ANI==0||r.ANI==5)&&y>=gety-road_size&&y<=gety+float(r.time-past)/1000*speed_per_second-road_size&&x>=getx-road_size&&x<=getx+road_size&&z==r.height)
            {
                last_road=k;
                return 1;
            }
            if((r.ANI==1||r.ANI==2||r.ANI==3)&&y>=gety-road_size&&y<=gety+min(float(r.time-past)/1000*speed_per_second,2*road_size)-road_size&&x>=getx-road_size&&x<=getx+road_size&&z==r.height)
            {
                last_road=k;
                return 1;
            }
            gety+=float(r.time-past)/1000*speed_per_second;
        }
        else if(b==1)
        {
            if((r.ANI==0||r.ANI==5)&&x>=getx-road_size&&x<=getx+float(r.time-past)/1000*speed_per_second-road_size&&y>=gety-road_size&&y<=gety+road_size&&z==r.height)
            {
                last_road=k;
                return 1;
            }
            if((r.ANI==1||r.ANI==2||r.ANI==3)&&x>=getx-road_size&&x<=getx+min(float(r.time-past)/1000*speed_per_second,2*road_size)-road_size&&y>=gety-road_size&&y<=gety+road_size&&z==r.height)
            {
                last_road=k;
                return 1;;
            }
            getx+=float(r.time-past)/1000*speed_per_second;
        }
        else if(b==2)
        {
            if((r.ANI==0||r.ANI==5)&&y<=gety+road_size&&y>=gety-float(r.time-past)/1000*speed_per_second+road_size&&x>=getx-road_size&&x<=getx+road_size&&z==r.height)
            {
                last_road=k;
                return 1;
            }
            if((r.ANI==1||r.ANI==2||r.ANI==3)&&y<=gety+road_size&&y>=gety-min(float(r.time-past)/1000*speed_per_second,2*road_size)+road_size&&x>=getx-road_size&&x<=getx+road_size&&z==r.height)
            {
                last_road=k;
                return 1;
            }
            gety-=float(r.time-past)/1000*speed_per_second;
        }
        else if(b==3)
        {
            if((r.ANI==0||r.ANI==5)&&x<=getx+road_size&&x>=getx-float(r.time-past)/1000*speed_per_second+road_size&&y>=gety-road_size&&y<=gety+road_size&&z==r.height)
            {
                last_road=k;
                return 1;
            }
            if((r.ANI==1||r.ANI==2||r.ANI==3)&&x<=getx+road_size&&x>=getx-min(float(r.time-past)/1000*speed_per_second,2*road_size)+road_size&&y>=gety-road_size&&y<=gety+road_size&&z==r.height)
            {
                last_road=k;
                return 1;;
            }
            getx-=float(r.time-past)/1000*speed_per_second;
        }
        past=r.time;
        k++;
    }
    return 0;
}




void make_road()
{
    int st=0,sz;
    int dir, dirs[2];
    dir=dirs[0]=zic_zac_diraction[0];
    dirs[1]=zic_zac_diraction[1];
    bool dir_flip=0;

    float x=0,y=0,z=0,x1,y1;
    float road_size=3,height=5;
    int col=0,k=0;
    float get;


    for(ROAD r:road)
    {
        if(k==0)
            road[k].before=0;
        else
            road[k].before=k-1;
        if(r.direction!=dir)
        {
            if(r.direction==dirs[!dir_flip])
                dir_flip=!dir_flip;
            if(r.direction!=-1)
            {
                dirs[dir_flip]=r.direction;
            }
            dir=dirs[dir_flip];
        }


        road[k].height=z;
        road[k].direction=dir;
        col=rand()%4+1;
        if(r.ANI==1||r.ANI==2||r.ANI==3)
        {
            if(float(r.time-st-6000/speed_per_second)/1000*speed_per_second<=2)
                r.ANI=0;
        }



        if(dir==0)
        {
            y1=y+float(r.time-st)/1000*speed_per_second;
            if(r.ANI==1||r.ANI==2||r.ANI==3||r.ANI==5)
                y1=min(y+2*road_size,y+float(r.time-st)/1000*speed_per_second);
            mainPoint.push_back(point{x-road_size,y-road_size,z});
            mainPoint.push_back(point{x+road_size,y-road_size,z});
            mainPoint.push_back(point{x+road_size,y1-road_size,z});
            mainPoint.push_back(point{x-road_size,y1-road_size,z});
            mainPoint.push_back(point{x-road_size,y-road_size,z-height});
            mainPoint.push_back(point{x+road_size,y-road_size,z-height});
            mainPoint.push_back(point{x+road_size,y1-road_size,z-height});
            mainPoint.push_back(point{x-road_size,y1-road_size,z-height});
            sz=mainPoint.size();
            mainPoly.push_back(poly{{sz-8,sz-7,sz-6,sz-5},1});
            mainPoly.push_back(poly{{sz-4,sz-3,sz-2,sz-1},col});
            mainPoly.push_back(poly{{sz-8,sz-4,sz-1,sz-5},col});
            mainPoly.push_back(poly{{sz-7,sz-3,sz-2,sz-6},col});
            mainPoly.push_back(poly{{sz-8,sz-4,sz-3,sz-7},col});
            mainPoly.push_back(poly{{sz-5,sz-1,sz-2,sz-6},col});
            mainObject.push_back(object{"road_"+to_string(k),(int)mainPoly.size()-6,6,(int)mainPoint.size()-8,8});
            if(r.ANI==1||r.ANI==2||r.ANI==3||r.ANI==5)
                y1=y+float(r.time-st)/1000*speed_per_second;
        }
        else if(dir==1)
        {
            x1=x+(float(r.time-st))/1000*speed_per_second;
            if(r.ANI==1||r.ANI==2||r.ANI==3||r.ANI==5)
                x1=min(x+2*road_size,x+(float(r.time-st))/1000*speed_per_second);
            mainPoint.push_back(point{x-road_size,y-road_size,z});
            mainPoint.push_back(point{x1-road_size,y-road_size,z});
            mainPoint.push_back(point{x1-road_size,y+road_size,z});
            mainPoint.push_back(point{x-road_size,y+road_size,z});
            mainPoint.push_back(point{x-road_size,y-road_size,z-height});
            mainPoint.push_back(point{x1-road_size,y-road_size,z-height});
            mainPoint.push_back(point{x1-road_size,y+road_size,z-height});
            mainPoint.push_back(point{x-road_size,y+road_size,z-height});
            sz=mainPoint.size();
            mainPoly.push_back(poly{{sz-8,sz-7,sz-6,sz-5},1});
            mainPoly.push_back(poly{{sz-4,sz-3,sz-2,sz-1},col});
            mainPoly.push_back(poly{{sz-8,sz-4,sz-3,sz-7},col});
            mainPoly.push_back(poly{{sz-5,sz-1,sz-2,sz-6},col});
            mainPoly.push_back(poly{{sz-5,sz-1,sz-4,sz-8},col});
            mainPoly.push_back(poly{{sz-6,sz-2,sz-3,sz-7},col});
            mainObject.push_back(object{"road_"+to_string(k),(int)mainPoly.size()-6,6,(int)mainPoint.size()-8,8});
            if(r.ANI==1||r.ANI==2||r.ANI==3||r.ANI==5)
                x1=x+(float(r.time-st))/1000*speed_per_second;
        }
        else if(dir==2)
        {
            y1=y-float(r.time-st)/1000*speed_per_second;
            if(r.ANI==1||r.ANI==2||r.ANI==3||r.ANI==5)
                y1=max(y-2*road_size,y-float(r.time-st)/1000*speed_per_second);
            mainPoint.push_back(point{x-road_size,y+road_size,z});
            mainPoint.push_back(point{x+road_size,y+road_size,z});
            mainPoint.push_back(point{x+road_size,y1+road_size,z});
            mainPoint.push_back(point{x-road_size,y1+road_size,z});
            mainPoint.push_back(point{x-road_size,y+road_size,z-height});
            mainPoint.push_back(point{x+road_size,y+road_size,z-height});
            mainPoint.push_back(point{x+road_size,y1+road_size,z-height});
            mainPoint.push_back(point{x-road_size,y1+road_size,z-height});
            sz=mainPoint.size();
            mainPoly.push_back(poly{{sz-8,sz-7,sz-6,sz-5},1});
            mainPoly.push_back(poly{{sz-4,sz-3,sz-2,sz-1},col});
            mainPoly.push_back(poly{{sz-8,sz-4,sz-1,sz-5},col});
            mainPoly.push_back(poly{{sz-7,sz-3,sz-2,sz-6},col});
            mainPoly.push_back(poly{{sz-8,sz-4,sz-3,sz-7},col});
            mainPoly.push_back(poly{{sz-5,sz-1,sz-2,sz-6},col});
            mainObject.push_back(object{"road_"+to_string(k),(int)mainPoly.size()-6,6,(int)mainPoint.size()-8,8});
            if(r.ANI==1||r.ANI==2||r.ANI==3||r.ANI==5)
                y1=y-float(r.time-st)/1000*speed_per_second;

        }
        else if(dir==3)
        {
            x1=x-(float(r.time-st))/1000*speed_per_second;
            if(r.ANI==1||r.ANI==2||r.ANI==3||r.ANI==5)
                x1=max(x-2*road_size,x-(float(r.time-st))/1000*speed_per_second);
            mainPoint.push_back(point{x+road_size,y-road_size,z});
            mainPoint.push_back(point{x1+road_size,y-road_size,z});
            mainPoint.push_back(point{x1+road_size,y+road_size,z});
            mainPoint.push_back(point{x+road_size,y+road_size,z});
            mainPoint.push_back(point{x+road_size,y-road_size,z-height});
            mainPoint.push_back(point{x1+road_size,y-road_size,z-height});
            mainPoint.push_back(point{x1+road_size,y+road_size,z-height});
            mainPoint.push_back(point{x+road_size,y+road_size,z-height});
            sz=mainPoint.size();
            mainPoly.push_back(poly{{sz-8,sz-7,sz-6,sz-5},1});
            mainPoly.push_back(poly{{sz-4,sz-3,sz-2,sz-1},col});
            mainPoly.push_back(poly{{sz-8,sz-4,sz-3,sz-7},col});
            mainPoly.push_back(poly{{sz-5,sz-1,sz-2,sz-6},col});
            mainPoly.push_back(poly{{sz-5,sz-1,sz-4,sz-8},col});
            mainPoly.push_back(poly{{sz-6,sz-2,sz-3,sz-7},col});
            mainObject.push_back(object{"road_"+to_string(k),(int)mainPoly.size()-6,6,(int)mainPoint.size()-8,8});
            if(r.ANI==1||r.ANI==2||r.ANI==3||r.ANI==5)
                x1=x-(float(r.time-st))/1000*speed_per_second;

        }


        if(r.ANI==FALL)
        {
            z=z-GRAVITY*(float(r.time-st-6000/speed_per_second)/1000*speed_per_second)*(float(r.time-st-6000/speed_per_second)/1000*speed_per_second);

        }
        if(r.ANI==NORMAL_JUMP)
        {
            //do nothing
        }
        if(r.ANI==HALF_JUMP)
        {
            z=z+GRAVITY*(float(r.time-st-6000/speed_per_second)/1000*speed_per_second)*(float(r.time-st-6000/speed_per_second)/1000*speed_per_second);
        }
        road[k].index=mainObject.size()-1;
        road[k].after=k+1;
        x=x1;
        y=y1;
        st=r.time;
        if(r.direction!=dir)
            dir_flip=!dir_flip;
        r.direction=dir;
        k++;
    }
    road[k-1].after=0;

}



void draw_road_line(point pt1,point pt2,int b,bool push=0)
{
    object ob;
    if(b==0)
    {
        point ptarr[]= {{pt1.x-1,pt1.y-1,pt1.z},{pt1.x+1,pt1.y-1,pt1.z},{pt1.x+1,pt1.y-1,pt1.z+2},{pt1.x-1,pt1.y-1,pt1.z+2},
            {pt2.x-1,pt2.y-1,pt1.z},{pt2.x+1,pt2.y-1,pt2.z},{pt2.x+1,pt2.y-1,pt2.z+2},{pt2.x-1,pt2.y-1,pt2.z+2}
        };
        poly P[]= {{{0,1,2,3}},{{4,5,6,7}},{{0,1,5,4}},{{2,3,7,6}},{{1,2,6,5}},{{0,3,7,4}}};


        if(!push)
        {
            glColor4f(mainColor[0].r,mainColor[0].g,mainColor[0].b,mainColor[0].a);
            for(int i=0; i<6; i++)
            {
                glBegin(GL_POLYGON);
                for(auto y:P[i].point_at)
                {
                    vect get=vect{ptarr[y].x,ptarr[y].y,ptarr[y].z};
                    glVertex3f(get.a,get.b,get.c);
                }
                glEnd();
            }
        }
        else
        {
            sz=mainPoint.size();
            for(point xx:ptarr)
                mainPoint.push_back(xx);

            for(poly xx:P)
            {
                for(int i=0; i<4; i++)
                {
                    xx.point_at[i]+=sz;
                }
                mainPoly.push_back(xx);
            }
            mainObject.push_back(object{"line_road",(int)mainPoly.size()-6,6,(int)mainPoint.size()-8,8,0});

        }
    }
    else if(b==1)
    {
        point ptarr[]= {{pt1.x-1,pt1.y+1,pt1.z},{pt1.x-1,pt1.y-1,pt1.z},{pt1.x-1,pt1.y-1,pt1.z+2},{pt1.x-1,pt1.y+1,pt1.z+2},
            {pt2.x-1,pt2.y+1,pt1.z},{pt2.x-1,pt2.y-1,pt2.z},{pt2.x-1,pt2.y-1,pt2.z+2},{pt2.x-1,pt2.y+1,pt2.z+2}
        };
        poly P[]= {{{0,1,2,3}},{{4,5,6,7}},{{0,1,5,4}},{{2,3,7,6}},{{1,2,6,5}},{{0,3,7,4}}};
        if(!push)
        {
            glColor4f(mainColor[0].r,mainColor[0].g,mainColor[0].b,mainColor[0].a);
            for(int i=0; i<6; i++)
            {
                glBegin(GL_POLYGON);
                for(auto y:P[i].point_at)
                {
                    vect get=vect{ptarr[y].x,ptarr[y].y,ptarr[y].z};
                    glVertex3f(get.a,get.b,get.c);
                }
                glEnd();
            }
        }
        else
        {
            sz=mainPoint.size();
            for(point xx:ptarr)
                mainPoint.push_back(xx);

            for(poly xx:P)
            {
                for(int i=0; i<4; i++)
                {
                    xx.point_at[i]+=sz;
                }
                mainPoly.push_back(xx);

            }
            mainObject.push_back(object{"line_road",(int)mainPoly.size()-6,6,(int)mainPoint.size()-8,8,0});

        }
    }
    else if(b==2)
    {
        point ptarr[]= {{pt1.x-1,pt1.y+1,pt1.z},{pt1.x+1,pt1.y+1,pt1.z},{pt1.x+1,pt1.y+1,pt1.z+2},{pt1.x-1,pt1.y+1,pt1.z+2},
            {pt2.x-1,pt2.y+1,pt1.z},{pt2.x+1,pt2.y+1,pt2.z},{pt2.x+1,pt2.y+1,pt2.z+2},{pt2.x-1,pt2.y+1,pt2.z+2}
        };
        poly P[]= {{{0,1,2,3}},{{4,5,6,7}},{{0,1,5,4}},{{2,3,7,6}},{{1,2,6,5}},{{0,3,7,4}}};


        if(!push)
        {
            glColor4f(mainColor[0].r,mainColor[0].g,mainColor[0].b,mainColor[0].a);
            for(int i=0; i<6; i++)
            {
                glBegin(GL_POLYGON);
                for(auto y:P[i].point_at)
                {
                    vect get=vect{ptarr[y].x,ptarr[y].y,ptarr[y].z};
                    glVertex3f(get.a,get.b,get.c);
                }
                glEnd();
            }
        }
        else
        {
            sz=mainPoint.size();
            for(point xx:ptarr)
                mainPoint.push_back(xx);

            for(poly xx:P)
            {
                for(int i=0; i<4; i++)
                {
                    xx.point_at[i]+=sz;
                }
                mainPoly.push_back(xx);
            }
            mainObject.push_back(object{"line_road",(int)mainPoly.size()-6,6,(int)mainPoint.size()-8,8,0});

        }
    }
    else if(b==3)
    {
        point ptarr[]= {{pt1.x+1,pt1.y+1,pt1.z},{pt1.x+1,pt1.y-1,pt1.z},{pt1.x+1,pt1.y-1,pt1.z+2},{pt1.x+1,pt1.y+1,pt1.z+2},
            {pt2.x+1,pt2.y+1,pt1.z},{pt2.x+1,pt2.y-1,pt2.z},{pt2.x+1,pt2.y-1,pt2.z+2},{pt2.x+1,pt2.y+1,pt2.z+2}
        };
        poly P[]= {{{0,1,2,3}},{{4,5,6,7}},{{0,1,5,4}},{{2,3,7,6}},{{1,2,6,5}},{{0,3,7,4}}};
        if(!push)
        {
            glColor4f(mainColor[0].r,mainColor[0].g,mainColor[0].b,mainColor[0].a);
            for(int i=0; i<6; i++)
            {
                glBegin(GL_POLYGON);
                for(auto y:P[i].point_at)
                {
                    vect get=vect{ptarr[y].x,ptarr[y].y,ptarr[y].z};
                    glVertex3f(get.a,get.b,get.c);
                }
                glEnd();
            }
        }
        else
        {
            sz=mainPoint.size();
            for(point xx:ptarr)
                mainPoint.push_back(xx);

            for(poly xx:P)
            {
                for(int i=0; i<4; i++)
                {
                    xx.point_at[i]+=sz;
                }
                mainPoly.push_back(xx);

            }
            mainObject.push_back(object{"line_road",(int)mainPoly.size()-6,6,(int)mainPoint.size()-8,8,0});

        }
    }

}
void output_objects()
{
    ofstream xout("output.txt");
    for(object ob:mainObject)
    {
        xout<<"g "+ob.name+"_"+ob.name<<endl;
        for(int pt=ob.pointStart; pt<ob.pointStart+ob.pointLen; pt++)
            xout<<"v "<<mainPoint[pt].x<<" "<<mainPoint[pt].y<<" "<<mainPoint[pt].z<<endl;
        xout<<"s 0"<<endl;
        for(int i=ob.polyStart; i<ob.polyStart+ob.polyLen; i++)
        {
            xout<<"usemtl "+mainColor[mainPoly[i].colorIndex].name<<endl;
            xout<<"f ";
            for(int j:mainPoly[i].point_at)
                xout<<j+1<<" ";
            xout<<endl;

        }
    }
}


vect getVector(float x,float y,int b=0)
{
    if(b==1)
    {
        x-=3.141592653589/2;
        return vect{cos(x),sin(x),0};
    }
    if(b==2)
        y+=3.141592653589/2;
    return vect{cos(x)*cos(y),sin(x)*cos(y),sin(y)};
}
#include <SFML/Audio.hpp>
sf::SoundBuffer sfBuffer;
sf::Sound sfSound;

static void key(unsigned char key, int xx,int yy)
{
    switch (key)
    {
    case 'y':
    {
        cout<<on_the_road(x,y,z,3)<<endl;
        break;
    }
    case 'a':
    {
        angle1+=0.1;
        break;
    }
    case 'w':
    {
        angle2+=0.1;
        break;
    }
    case 'd':
    {
        angle1-=0.1;
        break;
    }
    case 's':
    {
        angle2-=0.1;
        break;
    }
    case ' ':
    {
        position=position.add({0,0,1});
        if(move_cube)
            mainObject[0].shift=mainObject[0].shift.add({0,0,1});
        break;
    }
    case 'z':
    {
        position=position.sub({0,0,1});
        if(move_cube)
            mainObject[0].shift=mainObject[0].shift.sub({0,0,1});

        break;
    }

    case '1':
    {
        cout<<angle1<<" "<<angle2<<" "<<position.a<<","<<position.b<<","<<position.c<<endl;
        break;
    }
    case '2':
    {
        cout<<road[last_road].after<<"<<\n";
        if(!game_start)
        {
            game_start=1;

            start=move_start=timeSinceEpochMillisec();
            if(situation!=EDIT_MODE){
                if (sfBuffer.loadFromFile("theMaze.wav")) {
                    sfSound.setBuffer(sfBuffer);
                    sfSound.setLoop(true);
                    sfSound.play();
                    cout << "Playing music with SFML" << endl;
                } else {
                    cout << "Failed to load music file" << endl;
                }
            }
        }
        else if(situation==NORMAL||situation==ROAD_MAKING_MODE)
        {
            draw_road_line(last_position,point{x,y,z},dir,1);
            last_position= {x,y,z};
            if(dir==road[last_road].direction)
                dir=road[road[last_road].after].direction;
            else
                dir=road[last_road].direction;
            mainPoint.push_back(point{x+0.5,y+0.5,z});
            mainPoint.push_back(point{x-0.5,y+0.5,z});
            mainPoint.push_back(point{x-0.5,y-0.5,z});
            mainPoint.push_back(point{x+0.5,y-0.5,z});
            int sz=mainPoint.size();
            mainPoly.push_back(poly{{sz-4,sz-3,sz-2,sz-1}});
            mainObject.push_back(object{"",(int)mainPoly.size()-1,1});
            times.push_back(timeSinceEpochMillisec()-start);
        }
        break;
    }
    case '3':
    {
        for(uint64_t t:times)
            cout<<"{"<<t<<",0,-1},";
        cout<<">>"<<endl;
        break;
    }
    case 'r':
    {
        cout<<last_road<<"   "<<road[last_road].height<<endl;
    }

    }

    glutPostRedisplay();
}

static void specialKey(int key, int xx, int yy)
{
    switch (key)
    {

    case GLUT_KEY_LEFT:
    {
        position=position.sub(getVector(angle1,angle2,1).onPlane());
        if(move_cube)
            mainObject[0].shift=mainObject[0].shift.sub(getVector(angle1,angle2,1).onPlane());
        break;
    }
    case GLUT_KEY_UP:
    {
        position=position.add(getVector(angle1,angle2,0).onPlane());
        if(move_cube)
            mainObject[0].shift=mainObject[0].shift.add(getVector(angle1,angle2,0).onPlane());
        break;
    }
    case GLUT_KEY_RIGHT:
    {
        position=position.add(getVector(angle1,angle2,1).onPlane());
        if(move_cube)
            mainObject[0].shift=mainObject[0].shift.add(getVector(angle1,angle2,1).onPlane());
        break;
    }
    case GLUT_KEY_DOWN:
    {
        position=position.sub(getVector(angle1,angle2,0).onPlane());
        if(move_cube)
            mainObject[0].shift=mainObject[0].shift.sub(getVector(angle1,angle2,0).onPlane());
        break;
    }
    }
    glutPostRedisplay();
}
float t;

void loadFile(string fileName,vect scale= {1,1,1},vect shift= {0,0,0}) //only one file
{

    ifstream in(fileName);
    if(!in)
    {
        cout<<"file :\""+fileName+"\" not found"<<endl;
        return;
    }
    string line,get="",getColorName="";
    vector<string> words;
    int Npoint=mainPoint.size(),Ntex=texPoint.size(),Npoly=mainPoly.size(),Nobj=mainObject.size();
    int sz;
    int k;//used as a container
    while(getline(in,line))
    {
        line=line+" ";
        for(int i=0; i<line.length(); i++)
        {
            if(line[i]==' '&&get!="")
            {
                words.push_back(get);
                get="";
            }
            else
            {
                get+=line[i];
            }
        }

        get="";

        if(words[0]=="v")
        {
            mainObject[Nobj-1].pointLen++;
            mainPoint.push_back(point{stof(words[1])/scale.a,stof(words[2])/scale.b,stof(words[3])/scale.c});
        }
        if(words[0]=="vt")
        {
            if(words.size()==3)
                words.push_back("0");
            texPoint.push_back(point{stof(words[1]),stof(words[2]),stof(words[3])});
        }
        else if(words[0]=="f")
        {
            mainObject[Nobj-1].polyLen++;
            sz=mainPoly.size();
            mainPoly.push_back(poly());
            for(int i=1; i<words.size(); i++)
            {
                k=words[i].length();
                for(int j=0; j<words[i].length(); j++)
                    if(words[i][j]=='/')
                        k=j;
                mainPoly[sz].point_at.push_back(Npoint+stoi(words[i].substr(0,k))-1);
                if(k!=words[i].length())
                {
                    mainPoly[mainPoly.size()-1].textured=1;
                    mainPoly[sz].tex_point_at.push_back(Ntex+stoi(words[i].substr(k+1,words[i].length()-k-1))-1);
                }
            }
            mainPoly[sz].colorIndex=nameToColor[getColorName];
        }
        else if(words[0]=="newmtl")
        {
            getColorName=words[1];
            nameToColor[words[1]]=mainColor.size();
            mainColor.push_back(color{"",1,0,0,1});
        }
        else if(words[0]=="Kd"||words[0]=="kd")
        {

            mainColor[mainColor.size()-1]=color{"",stof(words[1]),stof(words[2]),stof(words[3]),1};
        }
        else if(words[0]=="usemtl")
        {
            getColorName=words[1];
        }
        else if(words[0]=="d")
        {
            //cout<<mainColor.size();
            //mainColor[mainColor.size()-1].a=stof(words[1]);
        }
        if(words[0]=="g")
        {

            mainObject.push_back(object{words[1].substr(0,words[1].length()/2),(int)mainPoly.size(),0,(int)mainPoint.size(),0,shift});
            Nobj++;
        }
        words.clear();
    }
}

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // DISABLE CULLING - all faces will show
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);  // Allow writing to depth buffer
    glDepthFunc(GL_LEQUAL);
    cen=getVector(angle1,angle2,0).add(position);
    up=getVector(angle1,angle2,2);
    glLoadIdentity();
    gluLookAt(position.a,position.b,position.c,cen.a,cen.b,cen.c,up.a,up.b,up.c);


////////////////////////////////////////
    /*
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);
        glLightfv(GL_LIGHT0,GL_SPOT_EXPONENT,&light_power);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);

    //Use depth buffering for hidden surface elimination
        glEnable(GL_DEPTH_TEST);*/


    //////////////////////////////
    glTranslatef(-4.0f, 0.0f, -7.0f);
    color getColor;
    vect get;


    for(int i=0; i<mainObject.size(); i++)
    {
        for(int x=mainObject[i].polyStart; x<mainObject[i].polyStart+mainObject[i].polyLen; x++)
        {
            if(mainPoly[x].colorIndex>=0)
            {
                getColor=mainColor[mainPoly[x].colorIndex];
                glColor4f(getColor.r,getColor.g,getColor.b,getColor.a);

            }
            else
            {
                glColor3f(1,1,1);
            }


            glBegin(GL_POLYGON);
            sz=0;
            for(int y=0; y<mainPoly[x].point_at.size(); y++)
            {
                get=vect{mainPoint[mainPoly[x].point_at[y]].x+mainObject[i].shift.a,mainPoint[mainPoly[x].point_at[y]].y+mainObject[i].shift.b,mainPoint[mainPoly[x].point_at[y]].z+mainObject[i].shift.c};
                //if(mainPoly[x].textured)glTexCoord2f(texPoint[mainPoly[x].tex_point_at[y]].x,texPoint[mainPoly[x].tex_point_at[y]].y);
                glVertex3f(get.a,get.b,get.c);
            }

            glEnd();


        }

    }
    if(situation==NORMAL)
        draw_road_line(last_position,point{x,y,z},dir,0);
    glFlush();
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}


void reshape(GLsizei width, GLsizei height)    // GLsizei for non-negative integer
{
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(70.0f, aspect, 0.5f, 120.0f);
}

void idle()
{
    if(situation!=EDIT_MODE)
    {
        move_end=timeSinceEpochMillisec();

        int k=0;
        for (animation ani:animationStack)
        {
            if(ani.type==0&&ani.stop==0)
            {
                mainObject[ani.target_object].shift=(ani.shift_vector.dot(float(move_end-ani.lastTime)/1000*speed_per_second));
                if(move_end-start>=ani.end_time)
                    animationStack[k].stop=1;

            }
            k++;
        }
        if(situation==NORMAL&&on_the_road(x+1,y+1,z)==0&&on_the_road(x+1,y-1,z)==0&&on_the_road(x-1,y-1,z)==0&&on_the_road(x-1,y+1,z)==0)
        {

            if(road[last_road].ANI!=0)
            {
                situation=road[last_road].ANI;
                if(situation==NORMAL_JUMP)
                {
                    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<";
                    if(last_road==0)
                        jump_height=float(road[last_road].time-6000/speed_per_second)/1000*speed_per_second;
                    else
                        jump_height=float(road[last_road].time-road[last_road-1].time-6000/speed_per_second)/1000*speed_per_second;
                    jump_height/=2;
                    jump_height=jump_height*jump_height;

                    cout<<"   "<<jump_height<<endl;
                }
                if(situation==HALF_JUMP)
                {
                    if(last_road==0)
                        jump_height=float(road[last_road].time-6000/speed_per_second)/1000*speed_per_second;
                    else
                        jump_height=float(road[last_road].time-road[last_road-1].time-6000/speed_per_second)/1000*speed_per_second;
                    jump_height=jump_height*jump_height;
                }
            }
            else
                situation=FALL;
            draw_road_line(last_position,point{x,y,z},dir,1);
            animation_start_height=z;
            animation_start_time=move_end;
        }
        speed=float(move_end-move_start)/1000*speed_per_second;
        if(situation==NORMAL&&road[last_road].ANI==5&&(last_road==0&&move_end-start>=0||last_road!=0&&move_end-start>=road[last_road-1].time))
        {
            road[last_road].ANI=0;
            animationStack.push_back(moving(move_end,speed_per_second,vect{(float)1-(float)road[last_road].direction,(float)road[last_road].direction,0},road[last_road].index,road[last_road].time-6000/speed_per_second));
        }

        
        if(game_start)
        {
            if(dir==0)
            {
                mainObject[0].shift.b+=speed;
                position.b+=speed;
                y+=speed;

            }
            else if(dir==1)
            {
                mainObject[0].shift.a+=speed;
                position.a+=speed;
                x+=speed;
            }
            else if(dir==2)
            {
                mainObject[0].shift.b-=speed;
                position.b-=speed;
                y-=speed;
            }
            else if(dir==3)
            {
                mainObject[0].shift.a-=speed;
                position.a-=speed;
                x-=speed;
            }
        }

        if(situation==FALL)
        {
            z=animation_start_height-GRAVITY*(float(move_end-animation_start_time)/1000*speed_per_second)*(float(move_end-animation_start_time)/1000*speed_per_second);
            mainObject[0].shift.c=animation_start_height-GRAVITY*(float(move_end-animation_start_time)/1000*speed_per_second)*(float(move_end-animation_start_time)/1000*speed_per_second);

            position.c=17.6397+animation_start_height-GRAVITY*(float(move_end-animation_start_time)/1000*speed_per_second)*(float(move_end-animation_start_time)/1000*speed_per_second);

            if(z<=road[last_road+1].height){

                if(on_the_road(x,y,road[last_road+1].height)
                        ||on_the_road(x+1,y+1,road[last_road+1].height)
                        ||on_the_road(x+1,y-1,road[last_road+1].height)
                        ||on_the_road(x-1,y-1,road[last_road+1].height)
                        ||on_the_road(x-1,y+1,road[last_road+1].height))
                {
                    mainObject[0].shift.c=z=road[last_road].height;
                    position.c=17.6397+road[last_road].height;
                    situation=NORMAL;
                    last_position=point{x,y,z};
                }
            }
        }
        else
        if(situation==NORMAL_JUMP)
        {
            z=animation_start_height+GRAVITY*(jump_height-(float(move_end-animation_start_time)/1000*speed_per_second-sqrt(jump_height))*(float(move_end-animation_start_time)/1000*speed_per_second-sqrt(jump_height)));
            mainObject[0].shift.c=animation_start_height+GRAVITY*(jump_height-(float(move_end-animation_start_time)/1000*speed_per_second-sqrt(jump_height))*(float(move_end-animation_start_time)/1000*speed_per_second-sqrt(jump_height)));
            position.c=17.6397+animation_start_height+GRAVITY*(jump_height-(float(move_end-animation_start_time)/1000*speed_per_second-sqrt(jump_height))*(float(move_end-animation_start_time)/1000*speed_per_second-sqrt(jump_height)));
            if(z<=road[last_road+1].height)
            {
                if(on_the_road(x,y,road[last_road+1].height)
                        ||on_the_road(x+1,y+1,road[last_road+1].height)
                        ||on_the_road(x+1,y-1,road[last_road+1].height)
                        ||on_the_road(x-1,y-1,road[last_road+1].height)
                        ||on_the_road(x-1,y+1,road[last_road+1].height))
                {
                    mainObject[0].shift.c=z=road[last_road].height;
                    position.c=17.6397+road[last_road].height;
                    situation=NORMAL;
                    last_position=point{x,y,z};
                }
            }
        }
        else if(situation==HALF_JUMP)
        {
            z=animation_start_height+GRAVITY*(jump_height-((float(move_end-animation_start_time)/1000*speed_per_second)-sqrt(jump_height))*((float(move_end-animation_start_time)/1000*speed_per_second)-sqrt(jump_height)));
            mainObject[0].shift.c=animation_start_height+GRAVITY*(jump_height-((float(move_end-animation_start_time)/1000*speed_per_second)-sqrt(jump_height))*((float(move_end-animation_start_time)/1000*speed_per_second)-sqrt(jump_height)));
            position.c=17.6397+animation_start_height+GRAVITY*(jump_height-((float(move_end-animation_start_time)/1000*speed_per_second)-sqrt(jump_height))*((float(move_end-animation_start_time)/1000*speed_per_second)-sqrt(jump_height)));
            if(true)
            {
                if(dir==0&&on_the_road(x,y-1,road[last_road+1].height)
                   ||dir==1&&on_the_road(x-1,y,road[last_road+1].height)
                   ||dir==2&&on_the_road(x,y+1,road[last_road+1].height)
                   ||dir==3&&on_the_road(x+1,y,road[last_road+1].height)
                   )
                {
                    mainObject[0].shift.c=z=road[last_road].height;
                    position.c=17.6397+road[last_road].height;
                    situation=NORMAL;
                    last_position=point{x,y,z};
                }
            }
        }


        move_start=move_end;
    }
    speed=0.5;//for normal movement
    glutPostRedisplay();
}

int main(int argc, char** argv)
{

    srand(time(0));
    loadFile("world.mtl");
    loadFile("world.obj");
    make_road();
    //loadFile("3d_models\\cars\\car_04.mtl");
    //loadFile("3d_models\\cars\\car_04.obj",{.01,.01,.01},{});
    output_objects();
    cout<<nameToColor["Material.001"]<<endl;

    glutInit(&argc, argv);            // Initialize GLUT
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);  // Add GLUT_DEPTH explicitly
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("title");          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event

    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);
    initGL();



    glutMainLoop();

    return 0;
}
