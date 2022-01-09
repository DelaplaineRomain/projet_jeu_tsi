/*****************************************************************************\
 * TP CPE, 4ETI, TP synthese d'images
 * --------------
 *
 * Programme principal des appels OpenGL
 \*****************************************************************************/

#include "declaration.h"

/*****************************************************************************\
* variable globale                                                            *
\*****************************************************************************/

//identifiant des shaders
GLuint shader_program_id;
GLuint gui_program_id;

camera cam;

const int nb_obj = 5;
objet3d obj[nb_obj];

const int id_joueur_1 = 0;
int vie_joueur_1 = 3;
const int id_joueur_2 = 1;
int vie_joueur_2 = 3;

const int nb_char = 2;
objet3d liste_char[nb_char];
vec3 liste_pos_char[nb_char] = {vec3(-7.0,0.0,-7.0),vec3(7.0,0.0,7.0)};

const int nb_projectile_1 =16;
int current_nb_proj_1 = 0;
objet3d liste_proj_1[nb_projectile_1];
float liste_direction_1[nb_projectile_1];
const int nb_projectile_2 = 16;
int current_nb_proj_2 = 0;
objet3d liste_proj_2[nb_projectile_2];
float liste_direction_2[nb_projectile_2];

const int nb_mur = 33;
objet3d liste_mur[nb_mur];
vec3 liste_pos_mur[nb_mur] = {
    vec3(0.0,0.0,0.0),
    vec3(1.0,0.0,-2.0),
    vec3(1.0,0.0,-3.0),
    vec3(1.0,0.0,-4.0),
    vec3(1.0,0.0,-5.0),
    vec3(3.0,0.0,-6.0),
    vec3(4.0,0.0,-6.0),
    vec3(5.0,0.0,-1.0),
    vec3(5.0,0.0,-2.0),
    vec3(5.0,0.0,-3.0),
    vec3(4.0,0.0,2.0),
    vec3(5.0,0.0,2.0),
    vec3(6.0,0.0,2.0),
    vec3(3.0,0.0,4.0),
    vec3(3.0,0.0,6.0),
    vec3(3.0,0.0,7.0),
    vec3(3.0,0.0,8.0),
    vec3(-1.0,0.0,2.0),
    vec3(-1.0,0.0,3.0),
    vec3(-1.0,0.0,4.0),
    vec3(-1.0,0.0,5.0),
    vec3(-3.0,0.0,6.0),
    vec3(-4.0,0.0,6.0),
    vec3(-5.0,0.0,1.0),
    vec3(-5.0,0.0,2.0),
    vec3(-5.0,0.0,3.0),
    vec3(-4.0,0.0,-2.0),
    vec3(-5.0,0.0,-2.0),
    vec3(-6.0,0.0,-2.0),
    vec3(-3.0,0.0,-4.0),
    vec3(-3.0,0.0,-6.0),
    vec3(-3.0,0.0,-7.0),
    vec3(-3.0,0.0,-8.0),
};

const int nb_text = 4;
text text_to_draw[nb_text];

// initialisation des variables globales
float dL = 0.1f; // Vitesse de déplacement
float nouvellePosX;
float nouvellePosZ;
float nouvelle_pos_x_proj;
float nouvelle_pos_z_proj;

bool keyupP1_pressed=false;
bool keydownP1_pressed=false;
bool keyleftP1_pressed=false;
bool keyrightP1_pressed=false;
bool keyshootP1_pressed=false;

bool keyupP2_pressed=false;
bool keydownP2_pressed=false;
bool keyleftP2_pressed=false;
bool keyrightP2_pressed=false;
bool keyshootP2_pressed=false;

vec2 vecteur_rayon;

int valMax = 56;
float liste_boundaries[56] = {
 -7.8f,7.8f,-7.8f,7.8f
,-0.5f,0.5f,-0.5f,0.5f
,-3.5f,-2.5f,-7.8f,-5.5f
,-3.5f,-2.5f,-4.5f,-3.5f
,-6.5f,-3.5f,-2.5f,-1.5f
,2.5f,3.5f,5.5f,7.8f
,2.5f,3.5f,3.5f,4.5f
,3.5f,6.5f,1.5f,2.5f
,-5.5f,-4.5f,0.5f,3.5f
,-1.5f,-0.5f,1.5f,5.5f
,-4.5f,-2.5f,5.5f,6.5f
,4.5f,5.5f,-3.5f,-0.5f
,0.5f,1.5f,-5.5f,-1.5f
,2.5f,4.5f,-6.5f,-5.5f
};
bool valRen;

float delta_proj = 1.7f;

/*****************************************************************************\
* initialisation                                                              *
\*****************************************************************************/
static void init()
{
  shader_program_id = glhelper::create_program_from_file("shaders/shader.vert", "shaders/shader.frag"); CHECK_GL_ERROR();

  cam.projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
  // cam.tr.translation = vec3(0.0f, 1.0f,0.0f);
  cam.tr.translation = vec3(0.0f, 3.75f, 9.25f);
  cam.tr.rotation_center = vec3(0.0f, 10.0f, 0.5f);
  cam.tr.rotation_euler = vec3(1.10f, 0.0f, 0.0f);

  init_model_1();
  init_model_2();
  init_model_3();
  init_model_4();
  init_arene();
  
  for (int i=0 ; i<nb_char ; i++) {
      init_char(i,liste_pos_char);
  }

  for (int i = 0; i < nb_mur; i++) {
      init_mur(i, liste_pos_mur);
  }

  for (int i = 0; i < nb_projectile_1; i++) {
      init_proj_1(i);
  }

  for (int i = 0; i < nb_projectile_2; i++) {
      init_proj_2(i);
  }

  gui_program_id = glhelper::create_program_from_file("shaders/gui.vert", "shaders/gui.frag"); CHECK_GL_ERROR();

  text_to_draw[0].value = "Romain et Clement"; // TODO refaire le TGA à la limite
  text_to_draw[0].bottomLeft = vec2(-1.0,0.9);
  text_to_draw[0].topRight = vec2(-0.25, 3.0);
  init_text(text_to_draw);

  text_to_draw[1]=text_to_draw[0];
  text_to_draw[1].value = "Jeu Player vs Player";
  text_to_draw[1].bottomLeft.y = 0.8f;
  text_to_draw[1].topRight.y = 2.0f;
}

/*****************************************************************************\
* deroulement du jeu                                                          *
\*****************************************************************************/
bool check_fin() {
    bool ret = false;
    if ((vie_joueur_1 == 0)||(vie_joueur_2 == 0)) {
        ret = true;
    }
    return ret;
}

static void reset() {
    vie_joueur_1 = 3;
    vie_joueur_2 = 3;
    
    for (int i = 0; i < nb_char; i++) {
        init_char(i, liste_pos_char);
    }

    for (int i = 0; i < nb_projectile_1; i++) {
        init_proj_1(i);
    }

    for (int i = 0; i < nb_projectile_2; i++) {
        init_proj_2(i);
    }
}

static void affichage() {
    // Affichage joueur 1
    text_to_draw[2] = text_to_draw[0];
    char str_vie_j1[10];
    sprintf(str_vie_j1, "%d", vie_joueur_1);

    text_to_draw[2].value = str_vie_j1;
    text_to_draw[2].bottomLeft = vec2(-1.05, -1.0);
    text_to_draw[2].topRight = vec2(-0.85, -0.8);

    // Affichage joueur 2
    text_to_draw[3] = text_to_draw[2];
    char str_vie_j2[10];
    sprintf(str_vie_j2, "%d", vie_joueur_2);

    text_to_draw[3].value = str_vie_j2;
    text_to_draw[3].bottomLeft.x = 0.79;
    text_to_draw[3].topRight.x = 0.99;
}


/*****************************************************************************\
* gestion projectile                                                          *
\*****************************************************************************/
void gestion_projectile(int id_joueur)
{
    if (id_joueur == id_joueur_1) {
        for (int i = 0; i < nb_projectile_1; i++) {
            if (i == current_nb_proj_1) {
                liste_proj_1[i].visible = true;
                liste_proj_1[i].tr.translation = liste_char[id_joueur].tr.translation;

                liste_direction_1[i] = liste_char[id_joueur].tr.rotation_euler.y;
            }
        }
    }
    else if (id_joueur == id_joueur_2) {
        for (int i = 0; i < nb_projectile_2; i++) {
            if (i == current_nb_proj_2) {
                liste_proj_2[i].visible = true;
                liste_proj_2[i].tr.translation = liste_char[id_joueur].tr.translation;

                liste_direction_2[i] = liste_char[id_joueur].tr.rotation_euler.y;
            }
        }
    }
}

void mvt_projectile() {
    for (int i = 0; i < nb_projectile_1; i++) {
        if (liste_proj_1[i].visible == true) {
            nouvelle_pos_z_proj = liste_proj_1[i].tr.translation.z;
            nouvelle_pos_x_proj = liste_proj_1[i].tr.translation.x;

            float orientation = liste_direction_1[i];

            if (orientation == (float)-2.35) {
                // tir diagonale haut gauche
                nouvelle_pos_z_proj = liste_proj_1[i].tr.translation.z - dL * 1.2;
                nouvelle_pos_x_proj = liste_proj_1[i].tr.translation.x - dL * 1.2;
            }
            else if (orientation == (float)2.35) {
                // tir diagonale haut droite
                nouvelle_pos_z_proj = liste_proj_1[i].tr.translation.z - dL * 1.2;
                nouvelle_pos_x_proj = liste_proj_1[i].tr.translation.x + dL * 1.2;
            }
            else if (orientation == (float)3.14) {
                // tir haut
                nouvelle_pos_z_proj = liste_proj_1[i].tr.translation.z - dL * delta_proj;
            }
            else if (orientation == (float)-0.78) {
                // tir diagonale bas gauche
                nouvelle_pos_z_proj = liste_proj_1[i].tr.translation.z + dL * 1.2;
                nouvelle_pos_x_proj = liste_proj_1[i].tr.translation.x - dL * 1.2;
            }
            else if (orientation == (float)0.78) {
                // tir diagonale bas droite
                nouvelle_pos_z_proj = liste_proj_1[i].tr.translation.z + dL * 1.2;
                nouvelle_pos_x_proj = liste_proj_1[i].tr.translation.x + dL * 1.2;

            }
            else if (orientation == (float)0) {
                // tir bas
                nouvelle_pos_z_proj = liste_proj_1[i].tr.translation.z + dL * delta_proj;
            }
            else if (orientation == (float)-1.57) {
                // tir gauche
                nouvelle_pos_x_proj = liste_proj_1[i].tr.translation.x - dL * delta_proj;
            }
            else if (orientation == (float)1.57) {
                // tir droite
                nouvelle_pos_x_proj = liste_proj_1[i].tr.translation.x + dL * delta_proj;
            }

            if (boundaries(nouvelle_pos_x_proj, nouvelle_pos_z_proj)) {
                if (collision(id_joueur_1+1, nouvelle_pos_x_proj, nouvelle_pos_z_proj)) {
                    liste_proj_1[i].tr.translation.x = nouvelle_pos_x_proj;
                    liste_proj_1[i].tr.translation.z = nouvelle_pos_z_proj;
                }
                else {
                    init_proj_1(i);
                    current_nb_proj_1--;
                    vie_joueur_2--;
                }
            }
            else {
                init_proj_1(i);
                current_nb_proj_1--;
            }
        }
    }

    for (int i = 0; i < nb_projectile_2; i++) {
        if (liste_proj_2[i].visible == true) {
            nouvelle_pos_z_proj = liste_proj_2[i].tr.translation.z;
            nouvelle_pos_x_proj = liste_proj_2[i].tr.translation.x;

            float orientation = liste_direction_2[i];

            if (orientation == (float)-2.35) {
                // tir diagonale haut gauche
                nouvelle_pos_z_proj = liste_proj_2[i].tr.translation.z - dL * 1.2;
                nouvelle_pos_x_proj = liste_proj_2[i].tr.translation.x - dL * 1.2;
            }
            else if (orientation == (float)2.35) {
                // tir diagonale haut droite
                nouvelle_pos_z_proj = liste_proj_2[i].tr.translation.z - dL * 1.2;
                nouvelle_pos_x_proj = liste_proj_2[i].tr.translation.x + dL * 1.2;
            }
            else if (orientation == (float)3.14) {
                // tir haut
                nouvelle_pos_z_proj = liste_proj_2[i].tr.translation.z - dL*delta_proj;
            }
            else if (orientation == (float)-0.78) {
                // tir diagonale bas gauche
                nouvelle_pos_z_proj = liste_proj_2[i].tr.translation.z + dL * 1.2;
                nouvelle_pos_x_proj = liste_proj_2[i].tr.translation.x - dL * 1.2;
            }
            else if (orientation == (float)0.78) {
                // tir diagonale bas droite
                nouvelle_pos_z_proj = liste_proj_2[i].tr.translation.z + dL * 1.2;
                nouvelle_pos_x_proj = liste_proj_2[i].tr.translation.x + dL * 1.2;

            }
            else if (orientation == (float)0) {
                // tir bas
                nouvelle_pos_z_proj = liste_proj_2[i].tr.translation.z + dL*delta_proj;
            }
            else if (orientation == (float)-1.57) {
                // tir gauche
                nouvelle_pos_x_proj = liste_proj_2[i].tr.translation.x - dL*delta_proj;
            }
            else if (orientation == (float)1.57) {
                // tir droite
                nouvelle_pos_x_proj = liste_proj_2[i].tr.translation.x + dL*delta_proj;
            }

            if (boundaries(nouvelle_pos_x_proj, nouvelle_pos_z_proj)) {
                if (collision(id_joueur_2 + 1, nouvelle_pos_x_proj, nouvelle_pos_z_proj)) {
                    liste_proj_2[i].tr.translation.x = nouvelle_pos_x_proj;
                    liste_proj_2[i].tr.translation.z = nouvelle_pos_z_proj;
                }
                else {
                    init_proj_2(i);
                    current_nb_proj_2--;
                    vie_joueur_1--;
                }
            }
            else {
                init_proj_2(i);
                current_nb_proj_2--;
            }
        }
    }
}

/*****************************************************************************\
* display_callback                                                           *
\*****************************************************************************/
 static void display_callback()
{
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();

  for(int i = 0; i < nb_obj; ++i)
    draw_obj3d(obj + i, cam);

  for(int i = 0; i < nb_char; ++i)
    draw_obj3d(liste_char + i, cam);

  for (int i = 0; i < nb_mur; ++i)
      draw_obj3d(liste_mur + i, cam);

  for(int i = 0; i < nb_text; ++i)
    draw_text(text_to_draw + i);

  for (int i = 0; i < nb_projectile_1; i++)
      draw_obj3d(liste_proj_1 + i, cam);

  for (int i = 0; i < nb_projectile_2; i++)
      draw_obj3d(liste_proj_2 + i, cam);

  glutSwapBuffers();
}

/*****************************************************************************\
* keyboard_callback                                                           *
\*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int)
{
  float delta = 0.5f;

  switch (key)
  {
    case 'p':
      glhelper::print_screen();
      break;
    case 27:
      exit(0);
      break;

      // Changement de la translation de la caméra
    /*
    case 'i':
        cam.tr.translation.x += delta;
        printf("translation x = %f \n",cam.tr.translation.x);
        break;
      case 'u':
        cam.tr.translation.x -= delta;
        printf("translation x = %f \n",cam.tr.translation.x);
        break;
      case 'k':
        cam.tr.translation.y += delta;
        printf("translation y = %f \n",cam.tr.translation.y);
        break;
      case 'j':
        cam.tr.translation.y -= delta;
        printf("translation y = %f \n",cam.tr.translation.y);
        break;
      case ';':
        cam.tr.translation.z += delta;
        printf("translation z = %f \n",cam.tr.translation.z);
        break;
      case ',':
        cam.tr.translation.z -= delta;
        printf("translation z = %f \n",cam.tr.translation.z);
        break;
      // Changement de rotation_center de la camera
      case 'y':
        cam.tr.rotation_center.x += delta;
        printf("rotation_center x = %f \n",cam.tr.rotation_center.x);
        break;
      case 't':
        cam.tr.rotation_center.x -= delta;
        printf("rotation _center x = %f \n",cam.tr.rotation_center.x);
        break;
      case 'h':
        cam.tr.rotation_center.y += delta;
        printf("rotation_center y = %f \n",cam.tr.rotation_center.y);
        break;
      case 'g':
        cam.tr.rotation_center.y -= delta;
        printf("rotation_center y = %f \n",cam.tr.rotation_center.y);
        break;
      case 'n':
        cam.tr.rotation_center.z += delta;
        printf("rotation_center z = %f \n",cam.tr.rotation_center.z);
        break;
      case 'b':
        cam.tr.rotation_center.z -= delta;
        printf("rotation_center z = %f \n",cam.tr.rotation_center.z);
        break;
      // Changement de rotation_euler de la camera
      case 'r':
        printf("avant x: %f",cam.tr.rotation_euler.x);
        cam.tr.rotation_euler.x += delta;
        printf("rotation_euler x = %f \n",cam.tr.rotation_euler.x);
        break;
      case 'e':
        cam.tr.rotation_euler.x -= delta;
        printf("rotation _euler x = %f \n",cam.tr.rotation_euler.x);
        break;
      case 'f':
        cam.tr.rotation_euler.y += delta;
        printf("rotation_euler y = %f \n",cam.tr.rotation_euler.y);
        break;
      case 'd':
        cam.tr.rotation_euler.y -= delta;
        printf("rotation_euler y = %f \n",cam.tr.rotation_euler.y);
        break;
      case 'v':
        cam.tr.rotation_euler.z += delta;
        printf("rotation_euler z = %f \n",cam.tr.rotation_euler.z);
        break;
      case 'c':
        cam.tr.rotation_euler.z -= delta;
        printf("rotation_euler z = %f \n",cam.tr.rotation_euler.z);
        break;

      case 'o':
          obj[4].tr.translation.y += 0.2;
          printf("res = %f \n", obj[4].tr.translation.y);
          break;
      case 'l':
          obj[4].tr.translation.y -= 0.2;
          printf("res = %f \n", obj[4].tr.translation.y);
          break;*/

    // Commandes Player 1
    
    case 'z':
      keyupP1_pressed = true;
      break;
    case 's':
      keydownP1_pressed = true;
      break;
    case 'q':
      keyleftP1_pressed = true;
      break;
    case 'd':
      keyrightP1_pressed = true;
      break;
    case 'x':
        keyshootP1_pressed = true;
        current_nb_proj_1++;
        gestion_projectile(id_joueur_1);
        break;

    // Commandes Player 2  
    case '8':
      keyupP2_pressed = true;
      break;
    case '5':
      keydownP2_pressed = true;
      break;
    case '4':
      keyleftP2_pressed = true;
      break;
    case '6':
      keyrightP2_pressed = true;
      break;
    case '2':
        keyshootP2_pressed = true;
        current_nb_proj_2++;
        gestion_projectile(id_joueur_2);
        break;
  }
}

static void keyboardup_callback(unsigned char key, int, int)
{
  switch (key)
  {
    // Commandes Player 1
    case 'z':
      keyupP1_pressed = false;
      break;
    case 's':
      keydownP1_pressed = false;
      break;
    case 'q':
      keyleftP1_pressed = false;
      break;
    case 'd':
      keyrightP1_pressed = false;
      break;
    case 'x':
      keyshootP1_pressed = false;
      //current_nb_proj_1++;
      //gestion_projectile(id_joueur_1);
      break;

    // Commandes Player 2  
    case '8':
      keyupP2_pressed = false;
      break;
    case '5':
      keydownP2_pressed = false;
      break;
    case '4':
      keyleftP2_pressed = false;
      break;
    case '6':
      keyrightP2_pressed = false;
      break;
    case '2':
      keyshootP2_pressed = false;
      //current_nb_proj_2++;  
      //gestion_projectile(id_joueur_2);
      break;
  }
}

/*****************************************************************************\
* special_callback                                                            *
\*****************************************************************************/
static void special_callback(int key, int, int)
{
}


/*****************************************************************************\
* timer_callback                                                              *
\*****************************************************************************/
static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
  //gestion du déplacement Player 1
  nouvellePosZ = liste_char[0].tr.translation.z;
  nouvellePosX = liste_char[0].tr.translation.x;
  if (keyupP1_pressed) {
    if (keyleftP1_pressed){
      liste_char[0].tr.rotation_euler.y = -2.35;
      // déplacement diagonale haut gauche
      nouvellePosX = liste_char[0].tr.translation.x - dL*0.7;
      nouvellePosZ = liste_char[0].tr.translation.z - dL*0.7;
    }else if (keyrightP1_pressed){
      // déplacement diagonale haut droite
      liste_char[0].tr.rotation_euler.y = 2.35;
      nouvellePosZ = liste_char[0].tr.translation.z - dL*0.7;
      nouvellePosX = liste_char[0].tr.translation.x + dL*0.7;
    }else{
      // déplacement vers le haut
      nouvellePosZ = liste_char[0].tr.translation.z - dL;
      liste_char[0].tr.rotation_euler.y = 3.14;
    }
  }else if (keydownP1_pressed) {
    if (keyleftP1_pressed){
      // déplacement diagonale bas gauche
      liste_char[0].tr.rotation_euler.y = -0.78;
      nouvellePosX = liste_char[0].tr.translation.x - dL*0.7;
      nouvellePosZ = liste_char[0].tr.translation.z + dL*0.7;
    }else if (keyrightP1_pressed){
      // déplacement diagonale bas droite
      liste_char[0].tr.rotation_euler.y = 0.78;
      nouvellePosX = liste_char[0].tr.translation.x + dL*0.7;
      nouvellePosZ = liste_char[0].tr.translation.z + dL*0.7;
    }else{
      // déplacement vers le bas
      nouvellePosZ = liste_char[0].tr.translation.z + dL;
      liste_char[0].tr.rotation_euler.y = 0;
    }
  }else if (keyleftP1_pressed) {
    // déplacement vers la gauche
    nouvellePosX = liste_char[0].tr.translation.x - dL;
    liste_char[0].tr.rotation_euler.y = -1.57;
  }else if (keyrightP1_pressed) {
    // déplacement vers la droite
    nouvellePosX = liste_char[0].tr.translation.x + dL;
    liste_char[0].tr.rotation_euler.y = 1.57;
  }
  if ((boundaries(nouvellePosX,nouvellePosZ)) && (collision(1,nouvellePosX,nouvellePosZ)) && (collision(3, nouvellePosX, nouvellePosZ))){
    liste_char[0].tr.translation.x = nouvellePosX;
    liste_char[0].tr.translation.z = nouvellePosZ;
  }
  //gestion du déplacement Player 2
  nouvellePosZ = liste_char[1].tr.translation.z;
  nouvellePosX = liste_char[1].tr.translation.x;
  if (keyupP2_pressed) {
    if (keyleftP2_pressed){
      // déplacement diagonale haut gauche
      liste_char[1].tr.rotation_euler.y = -2.35;
      nouvellePosX = liste_char[1].tr.translation.x - dL*0.7;
      nouvellePosZ = liste_char[1].tr.translation.z - dL*0.7;
    }else if (keyrightP2_pressed){
      // déplacement diagonale haut droite
      liste_char[1].tr.rotation_euler.y = 2.35;
      nouvellePosX = liste_char[1].tr.translation.x + dL*0.7;
      nouvellePosZ = liste_char[1].tr.translation.z - dL*0.7;
    }else{
      // déplacement vers le haut2
      nouvellePosZ = liste_char[1].tr.translation.z - dL;
      liste_char[1].tr.rotation_euler.y = 3.14;
    }
  }else if (keydownP2_pressed) {
    if (keyleftP2_pressed){
      // déplacement diagonale bas gauche
      liste_char[1].tr.rotation_euler.y = -0.78;
      nouvellePosZ = liste_char[1].tr.translation.z + dL*0.7;
      nouvellePosX = liste_char[1].tr.translation.x - dL*0.7;
    }else if (keyrightP2_pressed){
      // déplacement diagonale bas droite
      liste_char[1].tr.rotation_euler.y = 0.78;
      nouvellePosZ = liste_char[1].tr.translation.z + dL*0.7;
      nouvellePosX = liste_char[1].tr.translation.x + dL*0.7;
    }else{
      // déplacement vers le bas
      nouvellePosZ = liste_char[1].tr.translation.z + dL;
      liste_char[1].tr.rotation_euler.y = 0;
    }
  }else if (keyleftP2_pressed) {
    // déplacement vers la gauche
    nouvellePosX = liste_char[1].tr.translation.x - dL;
    liste_char[1].tr.rotation_euler.y = -1.57;
  }else if (keyrightP2_pressed) {
    // déplacement vers la droite
    nouvellePosX = liste_char[1].tr.translation.x + dL;
    liste_char[1].tr.rotation_euler.y = 1.57;
  }
  if ((boundaries(nouvellePosX,nouvellePosZ)) && (collision(2,nouvellePosX,nouvellePosZ)) && (collision(3, nouvellePosX, nouvellePosZ))){
    liste_char[1].tr.translation.x = nouvellePosX;
    liste_char[1].tr.translation.z = nouvellePosZ;
  }

  //Gestion des projectiles
  if (keyshootP1_pressed){
      //current_nb_proj_1++;
      //gestion_projectile(id_joueur_1);
  }

  if (keyshootP2_pressed){
    // current_nb_proj_2++;
    // gestion_projectile(id_joueur_2);
  }

  mvt_projectile();

  affichage();

  if (check_fin()) {
      printf("fin");
      reset();
  }
}

/*****************************************************************************\
* main                                                                         *
\*****************************************************************************/
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(600, 600);
  glutCreateWindow("OpenGL");

  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glutKeyboardUpFunc(keyboardup_callback);
  glutSpecialFunc(special_callback);
  glutTimerFunc(25, timer_callback, 0);

  glewExperimental = true;
  glewInit();

  std::cout << "OpenGL: " << (GLchar *)(glGetString(GL_VERSION)) << std::endl;

  init();
  glutMainLoop();

  return 0;
}

/*****************************************************************************\
* draw_text                                                                   *
\*****************************************************************************/
void draw_text(const text * const t)
{
  if(!t->visible) return;
  
  glDisable(GL_DEPTH_TEST);
  glUseProgram(t->prog);

  vec2 size = (t->topRight - t->bottomLeft) / float(t->value.size());
  
  GLint loc_size = glGetUniformLocation(gui_program_id, "size"); CHECK_GL_ERROR();
  if (loc_size == -1) std::cerr << "Pas de variable uniforme : size" << std::endl;
  glUniform2f(loc_size,size.x, size.y);     CHECK_GL_ERROR();

  glBindVertexArray(t->vao);                CHECK_GL_ERROR();
  
  for(unsigned i = 0; i < t->value.size(); ++i)
  {
    GLint loc_start = glGetUniformLocation(gui_program_id, "start"); CHECK_GL_ERROR();
    if (loc_start == -1) std::cerr << "Pas de variable uniforme : start" << std::endl;
    glUniform2f(loc_start,t->bottomLeft.x+i*size.x, t->bottomLeft.y);    CHECK_GL_ERROR();

    GLint loc_char = glGetUniformLocation(gui_program_id, "c"); CHECK_GL_ERROR();
    if (loc_char == -1) std::cerr << "Pas de variable uniforme : c" << std::endl;
    glUniform1i(loc_char, (int)t->value[i]);    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, t->texture_id);                            CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);                    CHECK_GL_ERROR();
  }
}

/*****************************************************************************\
* draw_obj3d                                                                  *
\*****************************************************************************/
void draw_obj3d(const objet3d* const obj, camera cam)
{
  if(!obj->visible) return;

  glEnable(GL_DEPTH_TEST);
  glUseProgram(obj->prog);
  // Gestion de camera
  {
    GLint loc_projection = glGetUniformLocation(shader_program_id, "projection"); CHECK_GL_ERROR();
    if (loc_projection == -1) std::cerr << "Pas de variable uniforme : projection" << std::endl;
    glUniformMatrix4fv(loc_projection,1,false,pointeur(cam.projection));    CHECK_GL_ERROR();

    GLint loc_rotation_view = glGetUniformLocation(shader_program_id, "rotation_view"); CHECK_GL_ERROR();
    if (loc_rotation_view == -1) std::cerr << "Pas de variable uniforme : rotation_view" << std::endl;
    mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation_z = matrice_rotation(cam.tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(loc_rotation_view,1,false,pointeur(rotation_x*rotation_y*rotation_z));    CHECK_GL_ERROR();

    vec3 cv = cam.tr.rotation_center;
    GLint loc_rotation_center_view = glGetUniformLocation(shader_program_id, "rotation_center_view"); CHECK_GL_ERROR();
    if (loc_rotation_center_view == -1) std::cerr << "Pas de variable uniforme : rotation_center_view" << std::endl;
    glUniform4f(loc_rotation_center_view , cv.x,cv.y,cv.z , 0.0f); CHECK_GL_ERROR();

    vec3 tv = cam.tr.translation;
    GLint loc_translation_view = glGetUniformLocation(shader_program_id, "translation_view"); CHECK_GL_ERROR();
    if (loc_translation_view == -1) std::cerr << "Pas de variable uniforme : translation_view" << std::endl;
    glUniform4f(loc_translation_view , tv.x,tv.y,tv.z , 0.0f); CHECK_GL_ERROR();
  }
  // Gestion de l'objet
  {
    GLint loc_rotation_model = glGetUniformLocation(obj->prog, "rotation_model"); CHECK_GL_ERROR();
    if (loc_rotation_model == -1) std::cerr << "Pas de variable uniforme : rotation_model" << std::endl;
    mat4 rotation_x = matrice_rotation(obj->tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(obj->tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation_z = matrice_rotation(obj->tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(loc_rotation_model,1,false,pointeur(rotation_x*rotation_y*rotation_z));    CHECK_GL_ERROR();

    vec3 c = obj->tr.rotation_center;
    GLint loc_rotation_center_model = glGetUniformLocation(obj->prog, "rotation_center_model");   CHECK_GL_ERROR();
    if (loc_rotation_center_model == -1) std::cerr << "Pas de variable uniforme : rotation_center_model" << std::endl;
    glUniform4f(loc_rotation_center_model , c.x,c.y,c.z , 0.0f);                                  CHECK_GL_ERROR();

    vec3 t = obj->tr.translation;
    GLint loc_translation_model = glGetUniformLocation(obj->prog, "translation_model"); CHECK_GL_ERROR();
    if (loc_translation_model == -1) std::cerr << "Pas de variable uniforme : translation_model" << std::endl;
    glUniform4f(loc_translation_model , t.x,t.y,t.z , 0.0f);                                     CHECK_GL_ERROR();
  }

  // On dessine l'objet
  glBindVertexArray(obj->vao);                                              CHECK_GL_ERROR();

  glBindTexture(GL_TEXTURE_2D, obj->texture_id);                            CHECK_GL_ERROR();
  glDrawElements(GL_TRIANGLES, 3*obj->nb_triangle, GL_UNSIGNED_INT, 0);     CHECK_GL_ERROR();
}

void init_text(text *t){
  vec3 p0=vec3( 0.0f, 0.0f, 0.0f);
  vec3 p1=vec3( 0.0f, 1.0f, 0.0f);
  vec3 p2=vec3( 1.0f, 1.0f, 0.0f);
  vec3 p3=vec3( 1.0f, 0.0f, 0.0f);

  vec3 geometrie[4] = {p0, p1, p2, p3}; 
  triangle_index index[2] = { triangle_index(0, 1, 2), triangle_index(0, 2, 3)};

  glGenVertexArrays(1, &(t->vao));                                              CHECK_GL_ERROR();
  glBindVertexArray(t->vao);                                                  CHECK_GL_ERROR();

  GLuint vbo;
  glGenBuffers(1, &vbo);                                                       CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo);                                          CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERROR();

  GLuint vboi;
  glGenBuffers(1,&vboi);                                                      CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi);                                 CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  t->texture_id = glhelper::load_texture("data/fontB.tga");

  t->visible = true;
  t->prog = gui_program_id;
}

GLuint upload_mesh_to_gpu(const mesh& m)
{
  GLuint vao, vbo, vboi;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1,&vbo);                                 CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo); CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), 0); CHECK_GL_ERROR();

  glEnableVertexAttribArray(1); CHECK_GL_ERROR();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_opengl), (void*)sizeof(vec3)); CHECK_GL_ERROR();

  glEnableVertexAttribArray(2); CHECK_GL_ERROR();
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(2*sizeof(vec3))); CHECK_GL_ERROR();

  glEnableVertexAttribArray(3); CHECK_GL_ERROR();
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(3*sizeof(vec3))); CHECK_GL_ERROR();

  glGenBuffers(1,&vboi); CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi); CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  return vao;
}

void init_model_1()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/char.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.27f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj[0].tr.rotation_center = vec3(0.0f,0.0f,0.0f);

  update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[0].vao = upload_mesh_to_gpu(m);

  obj[0].nb_triangle = m.connectivity.size();
  obj[0].texture_id = glhelper::load_texture("data/blue.tga");
  obj[0].visible = false;
  obj[0].prog = shader_program_id;

  obj[0].tr.translation = vec3(-2.0, 0.0, -10.0);
}

void init_model_4()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("data/mur.obj");

    // Affecte une transformation sur les sommets du maillage
    float s = 0.5f;
    mat4 transform = mat4(s, 0.0f, 0.0f, 0.0f,
        0.0f, 0.35f, 0.0f, 0.0f,
        0.0f, 0.0f, s, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    apply_deformation(&m, transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    obj[3].tr.rotation_center = vec3(0.0f, 0.0f, 0.0f);

    update_normals(&m);
    fill_color(&m, vec3(1.0f, 1.0f, 1.0f));

    obj[3].vao = upload_mesh_to_gpu(m);

    obj[3].nb_triangle = m.connectivity.size();
    obj[3].texture_id = glhelper::load_texture("data/paille.tga");
    obj[3].visible = false;
    obj[3].prog = shader_program_id;

    obj[3].tr.translation = vec3(0.0, 0.0, 0.0);
}

void init_model_2()
{

  mesh m;

  //coordonnees geometriques des sommets
/* Original ground coord vec3 p0=vec3(-25.0f,0.0f,-25.0f);
  vec3 p1=vec3( 25.0f,0.0f,-25.0f);
  vec3 p2=vec3( 25.0f,0.0f, 25.0f);
  vec3 p3=vec3(-25.0f,0.0f, 25.0f); */
  vec3 p0=vec3(-10.0f,0.0f,-10.0f);
  vec3 p1=vec3( 10.0f,0.0f,-10.0f);
  vec3 p2=vec3( 10.0f,0.0f, 10.0f);
  vec3 p3=vec3(-10.0f,0.0f, 10.0f);

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,1.0f,0.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  m.vertex = {v0, v1, v2, v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);  
  m.connectivity = {tri0, tri1};

  obj[1].nb_triangle = 2;
  obj[1].vao = upload_mesh_to_gpu(m);

  obj[1].texture_id = glhelper::load_texture("data/bois.tga");

  obj[1].visible = true;
  obj[1].prog = shader_program_id;
}


void init_model_3()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/projectile.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.27f;
  mat4 transform = mat4(s, 0.0f, 0.0f, 0.0f,
      0.0f, s, 0.0f, 0.0f,
      0.0f, 0.0f, s, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m, transform);

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj[2].tr.rotation_center = vec3(0.0f, 0.0f, 0.0f);

  update_normals(&m);
  fill_color(&m, vec3(1.0f, 1.0f, 1.0f));

  obj[2].vao = upload_mesh_to_gpu(m);

  obj[2].nb_triangle = m.connectivity.size();
  obj[2].texture_id = glhelper::load_texture("data/white.tga");
  obj[2].visible = false;
  obj[2].prog = shader_program_id;

  obj[2].tr.translation = vec3(-2.0, 0.0, -10.0);
}

void init_char(int i,vec3 *liste_pos_char)
{
  liste_char[i].vao = obj[0].vao;

  
  liste_char[i].nb_triangle = obj[0].nb_triangle;
  liste_char[i].texture_id = obj[0].texture_id;
  liste_char[i].visible = true;
  liste_char[i].prog = obj[0].prog;

  liste_char[i].tr.translation = liste_pos_char[i];
  if (i == 0) {
      liste_char[i].tr.rotation_euler.y = 0.78;
  }
  else if (i == 1) {
      liste_char[i].tr.rotation_euler.y = -2.35;
  }
}

void init_mur(int i, vec3 *liste_pos_mur)
{
    liste_mur[i].vao = obj[3].vao;

    liste_mur[i].nb_triangle = obj[3].nb_triangle;
    liste_mur[i].texture_id = obj[3].texture_id;
    liste_mur[i].visible = true;
    liste_mur[i].prog = obj[3].prog;
    liste_mur[i].tr.translation = liste_pos_mur[i];
}

void init_proj_1(int i)
{
    liste_proj_1[i].vao = obj[2].vao;

    liste_proj_1[i].nb_triangle = obj[2].nb_triangle;
    liste_proj_1[i].texture_id = obj[2].texture_id;
    liste_proj_1[i].visible = false;
    liste_proj_1[i].prog = obj[2].prog;

    liste_proj_1[i].tr.translation = obj[0].tr.translation;
    // liste_proj_1[i].tr.translation = liste_char[id_joueur].tr.translation;
}

void init_proj_2(int i)
{
    liste_proj_2[i].vao = obj[2].vao;

    liste_proj_2[i].nb_triangle = obj[2].nb_triangle;
    liste_proj_2[i].texture_id = obj[2].texture_id;
    liste_proj_2[i].visible = false;
    liste_proj_2[i].prog = obj[2].prog;

    liste_proj_2[i].tr.translation = obj[0].tr.translation;
    // liste_proj_2[i].tr.translation = liste_char[id_joueur].tr.translation;
}

void init_arene()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("data/arene.obj");

    // Affecte une transformation sur les sommets du maillage
    float s = 0.27f;
    mat4 transform = mat4(s, 0.0f, 0.0f, 0.0f,
        0.0f, s, 0.0f, 0.0f,
        0.0f, 0.0f, s, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    apply_deformation(&m, transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    obj[4].tr.rotation_center = vec3(0.0f, 0.0f, 0.0f);

    update_normals(&m);
    fill_color(&m, vec3(1.0f, 1.0f, 1.0f));

    obj[4].vao = upload_mesh_to_gpu(m);

    obj[4].nb_triangle = m.connectivity.size();
    obj[4].texture_id = glhelper::load_texture("data/white.tga");
    obj[4].visible = true;
    obj[4].prog = shader_program_id;

    obj[4].tr.translation = vec3(0.0, -1.35, 0.0);
    //obj[4].tr.translation = vec3(0.0, 0.5, 0.0);

}

bool boundaries(float posX, float posZ)
{   
    valRen = true;
    for (int i = 4; i < valMax; i = i+4) {
        if ((posX >= liste_boundaries[i]) && (posX <= liste_boundaries[i+1]) && (posZ >= liste_boundaries[i+2]) && (posZ <= liste_boundaries[i+3])) {
            valRen = false;
        }
    }
    if ((posX <= liste_boundaries[0]) || (posX >= liste_boundaries[1]) || (posZ <= liste_boundaries[2]) || (posZ >= liste_boundaries[3])) {
        valRen = false;
    }
    return valRen;
}

bool collision(int joueur,float posX,float posZ){
    valRen = true;
    if(joueur == 1){
        vecteur_rayon = vec2(posX,posZ) - vec2(liste_char[1].tr.translation.x,liste_char[1].tr.translation.z);
        if(norm(vecteur_rayon)<=1.15f){
            valRen = false;
        }
    } else if(joueur == 2){
        vecteur_rayon = vec2(posX,posZ) - vec2(liste_char[0].tr.translation.x,liste_char[0].tr.translation.z);
        if (norm(vecteur_rayon) <= 1.15f) {
            valRen = false;
        }
        
    } else if (joueur == 3) {
        for (int i = 0; i < nb_mur; ++i) {
            vecteur_rayon = vec2(posX, posZ) - vec2(liste_mur[i].tr.translation.x, liste_mur[i].tr.translation.z);
            if (norm(vecteur_rayon) <= 0.9f) {
                valRen = false;
            }
        }
    }
    return valRen;
}