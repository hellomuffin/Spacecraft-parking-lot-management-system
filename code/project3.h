//
//  project3.h
//  project3
//
//  Created by 郑晨浩 on 2020/8/16.
//  Copyright © 2020 郑晨浩. All rights reserved.
//

//• Teleported: have reserved slots marked by a rectangle randomly changing color;
//• UFO: device that spins on itself;
//• Spacecraft: vehicle which continuously zooms in and out;
//• Car: regular car;



#ifndef project3_h
#define project3_h

#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>
#include <GLUT/GLUT.h>
using namespace std;

const double pi=3.141592653;

const int UP_TEL=50;
const int UP_UFO=10;
const int UP_SPA=25;
const int UP_CAR=70;

const int UP=1;
const int DOWN=2;
const int RIGHT=3;
const int LEFT=4;

const int IN=-1;
const int OUT=-2;

const int CC=1;
const int C=0;

const int slot_num=10;

const int speed=2;
const int per_angle=1;

const int HOUR=200;


bool equal(float x,float y){
    if (x>y-0.0001 && x<y+0.0001) return true;
    else return false;
}



class Vec{
private:
    float x,y;
public:
    Vec(float _x=0, float _y=0) {
        x=_x; y=_y;
    }
    
    float getX() {return x;}
    float getY() {return y;}
    
    Vec operator+ (Vec v){
        return Vec(x+v.getX(),y+v.getY());
    }
    Vec operator- (Vec v){
        return Vec(x-v.getX(),y-v.getY());
    }
    Vec operator* (float k){
        return Vec(x*k,y*k);
    }
    float operator* (Vec v){
        return x*v.getX()+y*v.getY();
    }
    bool operator == (Vec &v){
        if (x-v.getX()>-2 || x-v.getX()<2) return true;
        else return false;
    }
    void rotate(float angle, Vec central, int dir){
        Vec temp=*this-central;
        float angle1;
        float angle2=angle/180*pi;
        float r=sqrt(pow(temp.x,2)+pow(temp.y,2));
        if (temp.x>0){
            angle1=atan(temp.y/temp.x);
        }
        else if (temp.x<0){
            angle1=pi+atan(temp.y/temp.x);            //-0.5*pi   1.5*pi
        }
        else{
            if (temp.y>0){
                angle1=0.5*pi;
            }
            else if (temp.y<0){
                angle1=1.5*pi;
            }
            else{
                return;
            }
        }
       
        if (dir==CC){
            Vec temp_res(r*cos(angle1+angle2),r*sin(angle1+angle2));
            x=temp_res.x+central.x;
            y=temp_res.y+central.y;
        }
        else{
            Vec temp_res(r*cos(angle1-angle2),r*sin(angle1-angle2));
            x=temp_res.x+central.x;
            y=temp_res.y+central.y;
        }
        return;
    }
};


class _door{
private:
    Vec cpoint;
    Vec epoint;
public:
    _door(): cpoint(354,941),epoint(220,941) {}
    void draw(){
        glColor3f((float)235/255,(float)51/255,(float)35/255);
        glLineWidth(5);
        glBegin(GL_LINES);
          glVertex2f(cpoint.getX(),cpoint.getY());
          glVertex2f(epoint.getX(),epoint.getY());
        glEnd();
    }
    
    void rotate(int mode){
        static int k=0;
        k++;
        if (k>0 && k<=45){
            if (mode==IN) epoint.rotate(2, cpoint, CC);
            else if (mode==OUT) epoint.rotate(2,cpoint,C);
        }
        else if (k>170 && k<=200){
            if (mode==IN) epoint.rotate(3, cpoint, C);
            else if (mode==OUT) epoint.rotate(3,cpoint,CC);
        }
        else if (k>=200){
            k=0;
        }
    }
};


void drawOutline(){
    float gray=136.0/255.0;
    glColor3f(gray,gray,gray);
    glLineWidth(5.0);
    glBegin(GL_LINE_STRIP);
      glVertex2f(110.0,941.0);
      glVertex2f(110.0,43.0);
      glVertex2f(1408.0,43.0);
      glVertex2f(1408.0,857.0);
      glVertex2f(353.0,857.0);
      glVertex2f(354.0,941.0);
    glEnd();
}

void drawSlot(){
    float gray=128.0/255.0;
    glColor3f(gray,gray,gray);
    glLineWidth(3.0);
    for (int i=0;i<6;i++){
        glBegin(GL_LINES);
        glVertex2f(354.0+161*i,857.0);
        glVertex2f(354.0+161*i,682.0);
        glEnd();
    }
    for (int i=0;i<6;i++){
        glBegin(GL_LINES);
        glVertex2f(354.0+161*i,46.0);
        glVertex2f(354.0+161*i,231.0);
        glEnd();
    }
}

void drawBlock(){
    float gray=128.0/255;
    glColor3f(gray,gray,gray);
    glRectf(395.0,411.0,1125.0,493.0);
}


class Park_lot{
private:
//    vector<Vec> slot[slot_num];
    vector<int> empty;
public:
    _door door;
    Park_lot (){
        //TODO: initialize the poistion of each place
        
        for (int i=0;i<slot_num;i++){
            empty.push_back(i);
        }
    }
    int find_empty(){
        if ((int)empty.size()==0){
            return -1;
        }
        else{
            int index=rand()%((int)empty.size()-1);
            return empty[index];
        }
    }
    void set_fill(int place){
        for (int i=0;i<empty.size();i++){
            if (empty[i]==place){
                empty.erase(empty.begin()+i);
            }
        }
    }
    void set_empty(int place){
        empty.push_back(place);
    }
    void draw(){
        door.draw();
        drawOutline();
        drawSlot();
        drawBlock();
    }
    
    
    
};



class Vehicle{
protected:
    int pass_time=0;
    int park_time;
    int slot;
    Vec anchor;
    vector<Vec> p;
public:
    int k=1; //angle/per_angle
    float position=1.0;
    int mode=IN;
    
    Vehicle (float _x, float _y): anchor(_x,_y), park_time(rand()%24+1) {}  //rand()%24+1
    
    virtual string type()=0;
    virtual int price()=0;
    
    Vec getAnchor() {return anchor;}
    
    int getParkTime() {return park_time;}
    
    void AddPassTime() {pass_time++;}
    int getPassTime() {return pass_time;}
    
    void setMode(int _mode) {mode=_mode;}
    int getMode() {return mode;}
    
    void setSlot(int s) {slot=s;}
    int getSlot() {return slot;}
    
    virtual void pointUpdate()=0;
    virtual Vec getCOM()=0;
    virtual void draw()=0;
    
    void move(int dir){
        if (dir==UP) anchor=anchor+Vec(0,-speed);
        else if (dir==DOWN) anchor=anchor+Vec(0,speed);
        else if (dir==LEFT) anchor=anchor+Vec(-speed,0);
        else if (dir==RIGHT) anchor=anchor+Vec(speed,0);
        pointUpdate();
    }
    
    virtual void rotate(int angle,int dir)=0;
    
    virtual void turn(Vec cpoint,int dir)=0;
//    virtual void zoom(float k)=0;

    virtual ~Vehicle() {}
};



class TEL: public Vehicle{
private:
    float red=(rand()%999+1)/1000.0, green=(rand()%999+1)/1000.0, blue=(rand()%999+1)/1000.0;
public:
    TEL(float _x=240,float _y=1093): Vehicle(_x,_y) {
        p.push_back(anchor+Vec(65,100));
        p.push_back(anchor+Vec(-65,100));
        p.push_back(anchor+Vec(-65,-100));
        p.push_back(anchor+Vec(65,-100));
    };
    
    void pointUpdate(){
        p[0]=anchor+Vec(65,100);
        p[1]=anchor+Vec(-65,100);
        p[2]=anchor+Vec(-65,-100);
        p[3]=anchor+Vec(65,-100);
    }
    
    
    string type() {return "teleporter";}
    int price() {return park_time*UP_TEL;}
    
    Vec getCOM(){
        return anchor;
    }
    
    void draw(){
        glColor3f(red,green,blue);
        glBegin(GL_POLYGON);
        for (int i=0;i<4;i++){
            glVertex2f(p[i].getX(),p[i].getY());
        }
        glEnd();
    }
    
    void rotate(int angle,int dir){
        for (int i=0;i<(int)p.size();i++){
            p[i].rotate(angle, anchor, dir);
        }
    }
    
    void turn(Vec cpoint,int dir){
        anchor.rotate(per_angle,cpoint,dir);
        pointUpdate();
    }
};


class UFO: public Vehicle{
public:
    UFO(float _x=240,float _y=1093): Vehicle(_x,_y) {
        float r=60;
        for (int i=0;i<20;i++){
            p.push_back(anchor+Vec(r*cos(1.05*pi/20*i), r*sin(1.05*pi/20*i)));
        }
        p.push_back(anchor-Vec(20,0));
        p.push_back(anchor-Vec(10,10));
        p.push_back(anchor-Vec(50,10));
        p.push_back(anchor-Vec(40,0)); //p[20]-p[23]
        p.push_back(anchor+Vec(20,0));
        p.push_back(anchor+Vec(10,-10));
        p.push_back(anchor+Vec(50,-10));
        p.push_back(anchor+Vec(40,0)); //p[24]-p[27]
        p.push_back(anchor+Vec(30,30*sqrt(3)));
        p.push_back(anchor+Vec(32,32*sqrt(3)-3));
        p.push_back(anchor+Vec(42,40*sqrt(3)-3));
        p.push_back(anchor+Vec(40,40*sqrt(3)));//p[28]-p[31]
        p.push_back(anchor+Vec(-30,30*sqrt(3)));
        p.push_back(anchor+Vec(-32,32*sqrt(3)-3));
        p.push_back(anchor+Vec(-42,40*sqrt(3)-3));
        p.push_back(anchor+Vec(-40,40*sqrt(3)));//p[32]-p[35]
    };
    
    void pointUpdate(){
        float r=60;
        for (int i=0;i<20;i++){
            p[i]=anchor+Vec(r*cos(1.05*pi/20*i), r*sin(1.05*pi/20*i));
        }
        p[20]=anchor-Vec(20,0);
        p[21]=anchor-Vec(10,10);
        p[22]=anchor-Vec(50,10);
        p[23]=anchor-Vec(40,0); //p[20]-p[23]
        p[24]=anchor+Vec(20,0);
        p[25]=anchor+Vec(10,-10);
        p[26]=anchor+Vec(50,-10);
        p[27]=anchor+Vec(40,0); //p[24]-p[27]
        p[28]=anchor+Vec(30,30*sqrt(3));
        p[29]=anchor+Vec(32,32*sqrt(3)-3);
        p[30]=anchor+Vec(42,40*sqrt(3)-3);
        p[31]=anchor+Vec(40,40*sqrt(3));//p[28]-p[31]
        p[32]=anchor+Vec(-30,30*sqrt(3));
        p[33]=anchor+Vec(-32,32*sqrt(3)-3);
        p[34]=anchor+Vec(-42,40*sqrt(3)-3);
        p[35]=anchor+Vec(-40,40*sqrt(3));//p[32]-p[35]
    }
    
    string type() {return "UFO";}
    int price() {return park_time*UP_UFO;}
    
    Vec getCOM(){
        return anchor;
    }
    
    void draw(){
        glColor3f(0,(float)33/255,(float)245/255);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 20; i++)
             {
                 glVertex2f(p[i].getX(),p[i].getY());
             }
        glEnd();
        
        glColor3f(1.0,(float)240/255,(float)81/255);
        glBegin(GL_POLYGON);
        for (int i = 20; i < 24; i++)
             {
                 glVertex2f(p[i].getX(),p[i].getY());
             }
        glEnd();
        glBegin(GL_POLYGON);
        for (int i = 24; i < 28; i++)
             {
                 glVertex2f(p[i].getX(),p[i].getY());
             }
        glEnd();
        
        glColor3f(1.0,0.0,0.0);
        glBegin(GL_POLYGON);
        for (int i = 28; i < 32; i++)
             {
                 glVertex2f(p[i].getX(),p[i].getY());
             }
        glEnd();
        
        glBegin(GL_POLYGON);
        for (int i = 32; i < 36; i++)
             {
                 glVertex2f(p[i].getX(),p[i].getY());
             }
        glEnd();
    }
    
    void rotate(int angle, int dir){
        for (int i=0;i<(int)p.size();i++){
            p[i].rotate(angle, anchor, dir);
        }
    }
    
    void turn(Vec cpoint,int dir){
        anchor.rotate(per_angle,cpoint,dir);
        pointUpdate();

    }
};


class SPA: public Vehicle{
public:
    SPA(float _x=156,float _y=1121): Vehicle(_x,_y) {
        p.push_back(anchor+Vec(84,-128));
        p.push_back(anchor+Vec(65,-94));
        p.push_back(anchor+Vec(104,-94));
        p.push_back(anchor+Vec(104,24)); //3
        p.push_back(anchor+Vec(65,24));  //4
        p.push_back(anchor+Vec(65,-31));
        p.push_back(anchor+Vec(36,24));
        p.push_back(anchor+Vec(104,-31)); //7
        p.push_back(anchor+Vec(132,24));
        p.push_back(anchor+Vec(70,24)); //9
        p.push_back(anchor+Vec(100,24));
        p.push_back(anchor+Vec(104,40));
        p.push_back(anchor+Vec(66,40));
    }
    //84 -28      240 1093                 156 1121
    void pointUpdate(){
        p[0]=anchor+Vec(84,-128);
        p[1]=anchor+Vec(65,-94);
        p[2]=anchor+Vec(104,-94);
        p[3]=anchor+Vec(104,24); //3
        p[4]=anchor+Vec(65,24);  //4
        p[5]=anchor+Vec(65,-31);
        p[6]=anchor+Vec(36,24);
        p[7]=anchor+Vec(104,-31); //7
        p[8]=anchor+Vec(132,24);
        p[9]=anchor+Vec(70,24); //9
        p[10]=anchor+Vec(100,24);
        p[11]=anchor+Vec(104,40);
        p[12]=anchor+Vec(66,40);
    }
    
    string type() {return "spacecraft";}
    int price() {return park_time*UP_SPA;}
    
    Vec getCOM(){
        return anchor+Vec(84,-128)+Vec(0,100);
    }
    
    void draw(){
        glColor3f((float)120/255,(float)176/255,(float)110/255);
        glBegin(GL_TRIANGLES);
        for (int i=0;i<3;i++){
            glVertex2f(p[i].getX(),p[i].getY());
        }
        glEnd();
        
        glColor3f((float)255/255,(float)240/255,(float)114/255);
        glBegin(GL_POLYGON);
        for (int i=1;i<5;i++){
            glVertex2f(p[i].getX(),p[i].getY());
        }
        glEnd();
        
        glColor3f((float)242/255,(float)157/255,(float)75/255);
        glBegin(GL_TRIANGLES);
        for (int i=4;i<7;i++){
            glVertex2f(p[i].getX(),p[i].getY());
        }
        glEnd();
        glBegin(GL_TRIANGLES);
          glVertex2f(p[3].getX(),p[3].getY());
          glVertex2f(p[7].getX(),p[7].getY());
          glVertex2f(p[8].getX(),p[8].getY());
        glEnd();
        
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_POLYGON);
        for (int i=9;i<13;i++){
            glVertex2f(p[i].getX(),p[i].getY());
        }
        glEnd();
        
    }

    void rotate(int angle,int dir){
        for (int i=0;i<(int)p.size();i++){
            p[i].rotate(angle, getCOM(), dir);
        }
    }
    
    void turn(Vec cpoint,int dir){
        Vec temp,cpoint_real;
        temp=cpoint-getCOM();
        cpoint_real=anchor+temp;
        anchor.rotate(per_angle,cpoint_real,dir);
        pointUpdate();
    }
};


class CAR: public Vehicle{
public:
    CAR(float _x=240.0,float _y=1038.0): Vehicle(_x,_y) {
        p.push_back(anchor-Vec(0,20));
        p.push_back(anchor+Vec(0,150));
        p.push_back(anchor+Vec(50,150));
        p.push_back(anchor+Vec(50,-20)); //p[0]-p[3]
        p.push_back(anchor+Vec(-35,40));
        p.push_back(anchor+Vec(0,10));
        p.push_back(anchor+Vec(0,120));
        p.push_back(anchor+Vec(-35,80));  //p[4]-p[7]
        for (int i=0;i<20;i++){
            p.push_back(anchor+Vec(50+10*cos(2*pi/20*i), 20+10*sin(2*pi/20*i)));
        }  //p[8]-p[27];
        for (int i=0;i<20;i++){
                p.push_back(anchor+Vec(50+10*cos(2*pi/20*i), 100+10*sin(2*pi/20*i)));
        }
    }
    
    void pointUpdate(){
        p[0]=anchor-Vec(0,20);
        p[1]=anchor+Vec(0,150);
        p[2]=anchor+Vec(50,150);
        p[3]=anchor+Vec(50,-20); //p[0]-p[3]
        p[4]=anchor+Vec(-35,40);
        p[5]=anchor+Vec(0,10);
        p[6]=anchor+Vec(0,120);
        p[7]=anchor+Vec(-35,80);  //p[4]-p[7]
        for (int i=8;i<28;i++){
            p[i]=anchor+Vec(50+10*cos(2*pi/20*i), 20+10*sin(2*pi/20*i));
        }  //p[8]-p[27];
        for (int i=28;i<48;i++){
            p[i]=anchor+Vec(50+10*cos(2*pi/20*i), 100+10*sin(2*pi/20*i));
        }
    }
    
    string type() {return "car";}
    int price() {return park_time*UP_CAR;}
    
    Vec getCOM(){
        return anchor+Vec(0,55);
    }
    
    void draw(){

        glBegin(GL_POLYGON);
          glColor3f((float)247/255,(float)194/255,(float)152/255);
          for (int i=0;i<4;i++){
              glVertex2f(p[i].getX(),p[i].getY());
          }
        glEnd();
        
        glBegin(GL_POLYGON);
          glColor3f((float)127/255,(float)144/255,(float)249/255);
          for (int i=4;i<8;i++){
              glVertex2f(p[i].getX(),p[i].getY());
          }
        glEnd();

        glColor3f((float)244/255,(float)152/255,(float)145/255);
        glBegin(GL_POLYGON);
        for (int i = 8; i < 28; i++)
             {
                 glVertex2f(p[i].getX(),p[i].getY());
             }
        glEnd();

        glBegin(GL_POLYGON);
        for (int i = 28; i < 48; i++)
             {
                 glVertex2f(p[i].getX(),p[i].getY());
             }
        glEnd();
    }
    
    void rotate(int angle,int dir){
        for (int i=0;i<(int)p.size();i++){
            p[i].rotate(angle, getCOM(), dir);
        }
    }
    
    void turn(Vec cpoint,int dir){
        Vec temp,cpoint_real;
        temp=cpoint-getCOM();
        cpoint_real=anchor+temp;
        anchor.rotate(per_angle,cpoint_real,dir);
        pointUpdate();
    }
};
#endif /* project3_h */
