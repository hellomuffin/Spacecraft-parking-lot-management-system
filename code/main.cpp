//
//  main.cpp
//  project3
//
//  Created by 郑晨浩 on 2020/8/16.
//  Copyright © 2020 郑晨浩. All rights reserved.
//

//Tasks to be completed:
//• Write a precise interface for all the classes from the regular parking lot;
//• Implement a function which finds an empty slot;
//• Implement a function which calculates the price to be paid on exiting the parking lot; • Implements two function to print a ticket for entering and exiting vehicles;

#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>
#include <GLUT/GLUT.h>
#include "project3.h"
using namespace std;

Park_lot PL;
vector<Vehicle*> vec;




string getCurTime()
 {
     time_t timep;
     time (&timep);
     char tmp[64];
     strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
     return tmp;
}


void arrTicket(Vehicle * vec, int empty){
    cout << "Time of arrival: " << getCurTime() << endl;
    cout << "Type of vehicle: " << vec->type() << endl;
    cout << "You can find an empty place at slot " << empty << endl;
    
}

void arrive(Vehicle *veco){
    int empty=PL.find_empty();
    arrTicket(veco,empty);
    veco->setSlot(empty);
    PL.set_fill(empty);
}

void depaTicket(Vehicle *veco){
    cout << "Time spent in the parking lot: " << veco->getParkTime() << endl;
    cout << "Type of vehicle: " << veco->type() <<endl;
    cout << "Price: " << veco->price() << endl;
}

void departure(Vehicle *veco){
    depaTicket(veco);
    PL.set_empty(veco->getSlot());
}

void judgePosition(Vehicle *veco){
    float x=veco->getCOM().getX();
    float y=veco->getCOM().getY();
    
    if (y>=1095){
        veco->position=1.0;
    }
    if (x>238 && x<242 && y>=690 && y<1093){
        veco->position=1.1;
    }
    else if (x<350 && y>=579 && y<690){
        veco->position=1.2;
    }
    else if (x>=350 && x<515 && y<662 && y>578){
        veco->position=1.3;
    }
    else if (x>=515 && x<676 && y<662 && y>578){
        veco->position=1.4;
    }
    else if (x>=676 && x<840 && y<662 && y>578){
        veco->position=1.5;
    }
    else if (x>=840 && x<1000 && y<662 && y>578){
        veco->position=1.6;
    }
    else if (x>=1000 && x<1164 && y<662 && y>578){
        veco->position=1.7;
    }
    else if (x>=1164 && x<=1166 && y>500){
        veco->position=4.0;
    }
    else if (x>=1164 && x<=1166 && y<500){
        veco->position=4.1;
    }
    else if (x>1166){
        veco->position=1.8;
    }
    else if (x>=1000 && x<1164 && y<332 && y>248){
        veco->position=1.9;
    }
    else if (x>=840 && x<1000 && y<332 && y>248){
        veco->position=1.101;
    }
    else if (x>=676 && x<840 && y<332 && y>248){
        veco->position=1.11;
    }
    else if (x>=515 && x<676 && y<332 && y>248){
        veco->position=1.12;
    }
    else if (x>=350 && x<515 && y<332 && y>248){
        veco->position=1.13;
    }
    else if (x>=350 && x<=1164 && y>=662 && y<740){
        veco->position=2.0;
    }
    else if (x>=350 && x<=1164 && y>=170 && y<=248){
        veco->position=2.1;
    }
    else if (x>=350 && x<=1164 && y>=740){
        veco->position=3.0;
    }
    else if (x>=350 && x<=1164 && y<=170){
        veco->position=3.1;
    }
}


void zoom0Judge(Vehicle *veco){
    if (veco->mode==IN){
        arrive(veco);
        veco->move(UP);
    }
    else if (veco->mode==OUT){
        departure(veco);
        delete veco;
        for (int i=0;i<(int)vec.size();i++){
            if (vec[i]==veco){
                vec.erase(vec.begin()+i);
                break;
            }
        }
    }
}

void zoom1Movement(Vehicle *veco){
    PL.door.rotate(veco->mode);
    if (veco->mode==IN){
        veco->move(UP);
    }
    else if (veco->mode==OUT){
        veco->move(DOWN);
    }
    
}

void zoom2Movement (Vehicle *veco){
    if (veco->mode==IN){
        veco->turn(Vec(350,690),CC);
        veco->rotate(per_angle*veco->k,CC);
        veco->k++;
    }
    else{
        veco->turn(Vec(350,690),C);
        veco->rotate(per_angle*veco->k,C);
        veco->k++;
    }
}

void zoom3Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==0){
            veco->turn(Vec(350,662),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,CC);
        }
    }
    else{
        if (veco->getSlot()==0){
            veco->turn(Vec(350,662),C);
            veco->rotate(per_angle*veco->k,C);
            veco->k++;
        }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
    
}

void zoom4Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==1){
            veco->turn(Vec(515,662),CC);
            veco->rotate(per_angle*veco->k,CC);
             veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,CC);
        }
    }
    else{
        if (veco->getSlot()==1){
            veco->turn(Vec(515,662),C);
            veco->rotate(per_angle*veco->k,C);
             veco->k++;
        }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
}

void zoom5Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==2){
            veco->turn(Vec(676,662),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,CC);
        }
    }
    else{
        if (veco->getSlot()==2){
            veco->turn(Vec(676,662),C);
            veco->rotate(per_angle*veco->k,C);
            veco->k++;
        }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
}

void zoom6Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==3){
            veco->turn(Vec(840,662),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,CC);
        }
    }
    else{
        if (veco->getSlot()==3){
            veco->turn(Vec(840,662),C);
            veco->rotate(per_angle*veco->k,C);
            veco->k++;
        }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
}

void zoom7Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==4){
            veco->turn(Vec(1000,662),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,CC);
        }
    }
   else{
       if (veco->getSlot()==4){
           veco->turn(Vec(1000,662),C);
           veco->rotate(per_angle*veco->k,C);
           veco->k++;
       }
       else{
           static int k=0;
           if (k==0) veco->k=-veco->k;
           else if (k==100) k=0;
           k++;
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
       }
    }
}

void zoom8Movement(Vehicle *veco){
    if (veco->mode==IN){
//        static int k=0;
//        if (k==0)        veco->k=-veco->k;
//        if (k>=179) k=0;
        //k++;
        veco->turn(Vec(1164,455),C);
        veco->rotate(per_angle*veco->k,C);
        veco->k++;
    }
    else{
//        static int j=0;
//        if (j==0)        veco->k=-veco->k;
//        j++;
//        if (j==100) j=0;
        veco->turn(Vec(1164,455),CC);
        veco->rotate(per_angle*veco->k,CC);
        veco->k++;
    }
}

void zoomAddiMovement(Vehicle *veco){
    if (veco->mode==IN){
         veco->k=-veco->k;
         veco->move(RIGHT);
         veco->rotate(per_angle*veco->k,C);
    }
    else if (veco->mode==OUT){
        veco->move(LEFT);
        veco->rotate(per_angle*veco->k,CC);
        
    }
}

void zoomAddiiMovement(Vehicle *veco){
    if (veco->mode==IN){
         veco->move(LEFT);
        veco->rotate(per_angle*veco->k,C);
    }
    else if (veco->mode==OUT){
        veco->k=-veco->k;
         veco->move(RIGHT);
        veco->rotate(per_angle*veco->k,CC);
    }
}

void zoom9Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==5){
            static int k=0;
            if (k==0) veco->k=-veco->k;
            else if (k==100) k=0;
            k++;
            veco->turn(Vec(1164,248),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
        }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
    else{
        if (veco->getSlot()==5){
            veco->turn(Vec(1164,248),C);
            veco->rotate(per_angle*veco->k,C);
            veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
}

void zoom10Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==6){
            static int k=0;
            if (k==0) veco->k=-veco->k;
            else if (k==100) k=0;
            k++;
            veco->turn(Vec(1000,248),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
        }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
    else{
        if (veco->getSlot()==6){
            veco->turn(Vec(1000,248),C);
            veco->rotate(per_angle*veco->k,C);
            veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
}

void zoom11Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==7){
             static int k=0;
            if (k==0) veco->k=-veco->k;
            else if (k==100) k=0;
            k++;
            veco->turn(Vec(840,248),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
        }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
    else{
        if (veco->getSlot()==7){
            veco->turn(Vec(840,248),C);
            veco->rotate(per_angle*veco->k,C);
            veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
}


void zoom12Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==8){
            static int k=0;
            if (k==0) veco->k=-veco->k;
            else if (k==100) k=0;
            k++;
            veco->turn(Vec(676,248),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
        }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
    else{
        if (veco->getSlot()==8){
            veco->turn(Vec(676,248),C);
            veco->rotate(per_angle*veco->k,C);
            veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
}

void zoom13Movement(Vehicle *veco){
    if (veco->mode==IN){
        if (veco->getSlot()==9){
             static int k=0;
            if (k==0) veco->k=-veco->k;
            else if (k==100) k=0;
            k++;
            veco->turn(Vec(515,248),CC);
            veco->rotate(per_angle*veco->k,CC);
            veco->k++;
         }
        else{
            veco->move(LEFT);
            veco->rotate(per_angle*veco->k,C);
        }
        }
    else{
        if (veco->getSlot()==9){
           veco->turn(Vec(515,248),C);
           veco->rotate(per_angle*veco->k,C);
           veco->k++;
        }
        else{
            veco->move(RIGHT);
            veco->rotate(per_angle*veco->k,C);
        }
    }
}

void toSlotMovement(int floor,Vehicle *veco){
    if (veco->mode==IN){
        if (floor==DOWN){
            veco->move(DOWN);
            veco->rotate(per_angle*veco->k,CC);
        }
        else{
            veco->move(UP);
            veco->rotate(per_angle*veco->k,CC);
        }
    }
    else{
        if (floor==UP){
            veco->move(DOWN);
            veco->rotate(per_angle*veco->k,CC);
        }
        else{
            veco->move(UP);
            veco->rotate(per_angle*veco->k,CC);
        }
    }
}

void inSlotMovement(int floor,Vehicle *veco){
    if (veco->getPassTime()==veco->getParkTime()*HOUR){
        veco->mode=OUT;
        if (floor==DOWN) {
            veco->move(UP);
            veco->rotate(per_angle*veco->k,CC);
        }
        else {
            veco->move(DOWN);
            veco->rotate(per_angle*veco->k,CC);
        }
    }
    else{
        
        veco->AddPassTime();
    }
}
                    


void display(){
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    PL.draw();
    for (int i=0;i<(int)vec.size();i++){
        vec[i]->draw();
    }

    glFlush();
    glutSwapBuffers();
}

void onTimer(int value){
    for (int i=0;i<(int)vec.size();i++){
        judgePosition(vec[i]);
        if (equal(vec[i]->position,1.0)){
            zoom0Judge(vec[i]);
        }
        else if (equal(vec[i]->position,1.1)){
            zoom1Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.2)){
            zoom2Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.3)){
            zoom3Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.4)){
            zoom4Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.5)){
            zoom5Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.6)){
            zoom6Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.7)){
            zoom7Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.8)){
            zoom8Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.9)){
            zoom9Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.101)){
            zoom10Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.11)){
            zoom11Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.12)){
            zoom12Movement(vec[i]);
        }
        else if (equal(vec[i]->position,1.13)){
            zoom13Movement(vec[i]);
        }
        else if (equal(vec[i]->position,2.0)){
            toSlotMovement(DOWN,vec[i]);
        }
        else if (equal(vec[i]->position,2.1)){
            toSlotMovement(UP, vec[i]);
        }
        else if (equal(vec[i]->position,3.0)){
            inSlotMovement(DOWN, vec[i]);
        }
        else if (equal(vec[i]->position,3.1)){
            inSlotMovement(UP, vec[i]);
        }
        else if (equal(vec[i]->position,4.0)){
            zoomAddiMovement(vec[i]);
        }
        else if (equal(vec[i]->position,4.1)){
            zoomAddiiMovement(vec[i]);
        }
        static int j=0;
        cout<<vec[i]->position<<" "<<j<<endl;
        j++;
//        vec[i]->rotate(1,CC);
    }
    
    glutPostRedisplay();
    glutTimerFunc(1,onTimer,1);
}

void processNormalKey(unsigned char key,int x,int y){
    if (key==49){
        vec.push_back(new TEL);
    }
    else if (key==50){
        vec.push_back(new UFO);
    }
    else if (key==51){
        vec.push_back(new SPA);
    }
    else if (key==52){
        vec.push_back(new CAR);
    }
    else if (key==27){
        exit(0);
    }
}




int main(int argc, char * argv[]) {
    vec.push_back(new UFO);
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1610,1256);
    glutInitWindowPosition(400, 400);
    glutCreateWindow("Project 3");
    gluOrtho2D(0.0,1610.0,1256.0,0.0);
    glutDisplayFunc(display);
    glutTimerFunc(1, onTimer, 1);
    glutKeyboardFunc(processNormalKey);
//    glutIdleFunc(idle);
    glutMainLoop();

//
//    vec.push_back(new CAR);
//    arrive(vec[0]);
//
//    vec.push_back(new SPA);
//    arrive(vec[1]);
//
//    vec.push_back(new UFO);
//    arrive(vec[2]);
//
//    vec.push_back(new TEL);
//    arrive(vec[3]);
//
//    vec.push_back(new TEL);
//    arrive(vec[4]);
//
//    cout<<endl;
//    for (int i=0;i<(int)PL.empty.size();i++){
//        cout<<PL.empty[i]<<" "<<endl;
//    }
//    cout<<endl;
//
//    departure(vec[0]);
//    departure(vec[4]);
//
//    cout<<endl;
//    for (int i=0;i<(int)PL.empty.size();i++){
//        cout<<PL.empty[i]<<" "<<endl;
//    }
    
    
    return 0;
}

