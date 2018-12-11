#include <irrlicht.h>

#include <iostream>
#include <string>
#include <vector>
#include "events.h"
#include "math.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

void character_motion(EventReceiver receiver,is::IAnimatedMeshSceneNode *node_pers,
		      bool &is_he_running, bool &is_he_standing, bool &jump, bool &avance, bool &recule,
		      float &motion_direction)
{
    ic::vector3df position = node_pers->getPosition();
    ic::vector3df rotation = node_pers->getRotation();
          
    if(receiver.avance)
    {
      if(motion_direction<0)
      {
       rotation.Y = 0;//+= 180;
       motion_direction = 1;
      }
      position.X += 1 ;
    }
    
    if(receiver.recule)
    {
      
     if(motion_direction>0)
     {
       rotation.Y = 180;
       motion_direction = -1;
     }
     
     position.X -= 1 ; 
    }
    
    if(receiver.recule)
    {   
     position.Y += 1 ; 
    }
    
    
    if(receiver.avance || receiver.recule) // si une touche de déplacement est activée
    { 
      is_he_standing = false;             // le personnage n'est pas debout
      if(!is_he_running)                  // s'il n'est pas déja en train de courir
      {
	is_he_running = node_pers->setMD2Animation(irr::scene::EMAT_RUN);  // il doit courir maintenant
      }
      
    }
    else                                  // si la touche de déplacement n'est pas activée
    {
      is_he_running = false;              // le personnage ne cours pas
      if(!is_he_standing)                 // s'il n'est pas déja debout
	is_he_standing = node_pers->setMD2Animation(irr::scene::EMAT_STAND); // il doit se tenir debout maintenant
    }
    
    
    if(receiver.jump) // si une touche de déplacement est activée
    { 
      is_he_standing = false;             // le personnage n'est pas debout
      if(!is_he_jumping)                  // s'il n'est pas déja en train de courir
      {
	is_he_jumping = node_pers->setMD2Animation(irr::scene::EMAT_JUMP);  // il doit courir maintenant
      }
      
    }
    else                                  // si la touche de déplacement n'est pas activée
    {
      is_he_jumping = false;              // le personnage ne cours pas
      if(!is_he_standing)                 // s'il n'est pas déja debout
	is_he_standing = node_pers->setMD2Animation(irr::scene::EMAT_STAND); // il doit se tenir debout maintenant
    }
    
    
    
    node_pers->setPosition(position);
    node_pers->setRotation(rotation);
}




int main()
{
  // Le gestionnaire d'événements
  EventReceiver receiver;
  
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(640, 480),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();

  // Ajout de l'archive qui contient entre autres un niveau complet
 device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");

  // On charge un bsp (un niveau) en particulier :
  is::IAnimatedMesh *mesh = smgr->getMesh("20kdm2.bsp");
  is::IMeshSceneNode *node = smgr->addOctreeSceneNode(mesh->getMesh(0));
  node->setDebugDataVisible(is::EDS_BBOX);
  node->setPosition(core::vector3df(-1350, -130, -1400));

  // On charge un personnage en particulier :
  is::IAnimatedMesh *mesh_pers = smgr->getMesh("data/tris.md2");
  is::IAnimatedMeshSceneNode *node_pers = smgr->addAnimatedMeshSceneNode(mesh_pers);
  receiver.set_node(node_pers);
  node_pers->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  node_pers->setMD2Animation(irr::scene::EMAT_STAND);
  ic::vector3df position_perso_init = node_pers->getPosition();
  
  bool jump = false;
  bool avance = false;
  bool recule = false;
  
  receiver.set_actions(jump,avance,recule);
  
  bool is_he_standing = true;
  bool is_he_running = false;
  
  // initialise la direction de déplacement (vecteur d'orientation) selon X
  float motion_direction = 1; 

  // application de la texture
  node_pers->setMaterialTexture(0,driver->getTexture("data/blue_texture.pcx"));


  // Création du triangle selector
  scene::ITriangleSelector *selector;
  selector = smgr->createOctreeTriangleSelector(node->getMesh(), node);
  node->setTriangleSelector(selector);

  // Création de la caméra
  //scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
  ic::vector3df position_cam = ic::vector3df(0, 0, -70);
  ic::vector3df orientation_cam = position_perso_init - position_cam;
  
   
  scene::ICameraSceneNode* camera =
  smgr->addCameraSceneNode(nullptr,
                           position_cam,         // position
                           orientation_cam,          // orientation
                           -1,          // Identifiant
                           true);        // camera active*/
			     
  
  // Et l'animateur/collisionneur
  /*scene::ISceneNodeAnimator *anim;
  anim = smgr->createCollisionResponseAnimator(selector, 
                                               camera,  // Le noeud que l'on veut gérer
                                               ic::vector3df(30, 50, 30), // "rayons" de la caméra
                                               ic::vector3df(0, 0, 0),  // gravité
                                               ic::vector3df(0, 30, 0));  // décalage du centre
  camera->addAnimator(anim);*/

  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(250,150,200,255));
    
    character_motion(receiver,node_pers,is_he_running,is_he_standing,
		     jump, avance, recule, motion_direction);
    
    ic::vector3df position_perso = node_pers->getPosition();
    
    camera->setPosition(position_perso - orientation_cam);
    camera->setTarget(position_perso);
    // Dessin de la scène :
    smgr->drawAll();
    // 
    driver->endScene();
  }
  device->drop();

  return 0;
}

