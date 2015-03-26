// Gameduino libraries needed
#include <SPI.h>
#include <GD.h>
#include "losseta.h"

# define NL 8 // nombre de losetes
# define LL 3 // longitud de les losetes, en blocs.
# define XMAX 384 // 400 - 16 pixels (suposem sprites mobils de 16x16)
# define YMAX 284 // 300 - 16 pixels (suposem sprites mobils de 16x16)
# define PSL 4 // primer sprite que forma part d'una loseta

int a;
int c;
int xp = 110;
int yp = 90;
int vxp = -2;
int vyp = -2;
int nl = NL;
int xl[NL] = {0, 48, 96, 144, 192, 240, 288, 336};
int yl[NL] = {20, 60, 60, 60, 40, 100, 40, 40};
int ll = LL;
int lxoc;
int lpeca;//¿¿¿???
int vives;
int j;
int xb = 200;
boolean xoc;
boolean xocb;
int oprt = 3;
boolean b;
boolean d;

void pinta_sprites() {
  
    GD.sprite(0, xp, yp, 12, 8);
    GD.sprite(1, xb, 260, 0, 8);
    GD.sprite(2, xb+16, 260, 1, 8);
    GD.sprite(3, xb+32, 260, 2, 8);
    for (int i = 0; i < nl; i++) {
        for (j = 0; j < ll; j++) { 
            GD.sprite(3*i+j+PSL, xl[i]+16*j, yl[i], j, 8);
        }    
    }
     
    /*
      GD.__wstartspr(0);
    draw_losseta(xp, yp, 0, 1); 
    draw_losseta(xb, 260, 0, 0);
    draw_losseta(xb+16, 260, 1, 0);
    draw_losseta(xb+32, 260, 2, 0);
    for (int i = 0; i < nl; i++) {
        for (j = 0; j < ll; j++) { 
            draw_losseta(xl[i]+16*j, yl[i], j, 0);
        }
    }
  GD.__end();*/
}

void integra_velocitat() {

    xp = xp + vxp;
    yp = yp + vyp;
    
}

void efecte_xoc() {



  lpeca = j;
    int deltax = abs(xp - xl[lxoc]);
    int deltay = abs(yp - (yl[lxoc]+16*lpeca));
    if (deltax >= deltay) vxp = -vxp;
    if (deltax <= deltay) vyp = -vyp;
    
// eliminem la loseta lxoc;

    vives = vives - 1;
    xl[lxoc] = 401;
    yl[lxoc] = 301;
    
}

void efecte_xocb(){
    int echox = abs(xp - xb);
    int echoy = abs(yp - (260+16*j));
    if (echox >= echoy) vxp = -vxp;
    if (echox <= echoy) vyp = -vyp;
}

void efecte_parets() {
 
     if (xp>XMAX || xp<0) vxp = -vxp;
     if (yp<20) vyp = -vyp;
     if (yp>320){
      xp = random(100, 300);  
      yp = 235;
      vyp = -2;
      oprt = oprt - 1;
  }
}

void setup() {

// Configuracio entrada  

    pinMode(3,INPUT);
    pinMode(4,INPUT);


  
// allow for setup

    delay(250);
    
    GD.begin();
    
    // load character set

    GD.ascii();
    
    // set background to black

    unsigned int bg_colour = RGB(0,0,0);
    GD.wr16(BG_COLOR, bg_colour);
    
    // set up PALETTE4A

    GD.wr16(PALETTE4A, RGB(255, 0, 0));
    GD.wr16(PALETTE4A + (1<<1), RGB(0, 255, 0));
    GD.wr16(PALETTE4A + (2<<1), RGB(0, 0, 255));
    
    // pixel with value of 3 will be transparent

    GD.wr16(PALETTE4A + (3<<1), TRANSPARENT);
    
    // copy the sprite image into RAM
    //GD.copy(RAM_SPRIMG, lossseta_sprpal, sizeof(lossseta_sprpal));
    
    //tenim una paleta de 4A
    GD.copy(PALETTE4A, losseta_sprpal, sizeof(losseta_sprpal));   
   
    //descomprimim el sprite
    GD.uncompress(RAM_SPRIMG, losseta_sprimg);
    
    // set the sprite control word

    pinta_sprites();    
         
}

void nivell(int nv) {
/*
if (nv == 1) {
    vives = 2;
    xl = {130, 260};
    yl = {50, 60};
    
} else if (nv == 2) {
    vives = 1;
    xl = {40, 401};
    yl = {150, 301}; 
}


while(vives > 0){
  
/* Llegir controls 1 cop per cicle
  
        a=digitalRead(4);
        b=digitalRead(3);
    
        c=digitalRead(5);
        d=digitalRead(6);
    
*/    
    
        GD.waitvblank();
        
        if((GD.rd(COLLISION+1)!=0xff) || (GD.rd(COLLISION+2)!=0xff) || (GD.rd(COLLISION+3)!=0xff)){
          xocb = true;
        }else{
          xocb = false;
        }
        
        xoc = false;
        for (int i = 0; i < nl; i++) {
            for (int j = 0; j < ll; j++) {  
                xoc = (GD.rd(COLLISION+3*i+j+PSL)!=0xff);
                if (xoc) {
                    lxoc = i;
                    lpeca = j;
                    i = nl;
                    j = ll;  // Amb "break" nomes sortiriem del primer bucle.
                }
            }
        }
        if (xocb) efecte_xocb();
        if (xoc) efecte_xoc();

        efecte_parets();

        integra_velocitat();

        pinta_sprites();

    }    
//}

void loop() {

nivell(1);  
nivell(2);
  
  a=digitalRead(3);
  c=digitalRead(4);
  
if(a==0 && b==false){
 if (xb<384) xb = xb+10;
 b = true;
}else if(a==0 && b==true){
 b = false;
}

if(c==0 && d==false){
 if (xb>0) xb = xb-10;
 d = true;
}else if(c==0 && d==true){
 d = false;
}


switch (oprt) {
        case 0:
        GD.putstr(1, 1, "0 vides");
        break;
        case 1:
        GD.putstr(1, 1, "1 vides");
        break;
        case 2:
        GD.putstr(1, 1, "2 vides");
        break;
        case 3:
        GD.putstr(1, 1, "3 vides");
        break;
  }
}

