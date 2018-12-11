// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>

#include <irrlicht.h>
using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


class EventReceiver : public irr::IEventReceiver
{
  is::IAnimatedMeshSceneNode *node;

  bool keyboard_handler(const irr::SEvent &event);
  bool gui_handler(const irr::SEvent &event);
public:
  EventReceiver();
  bool OnEvent(const irr::SEvent &event);
  void set_node(is::IAnimatedMeshSceneNode *node);
  void set_actions(bool &jump, bool &avance, bool &recule);
  bool jump = false;
  bool avance =false; 
  bool recule = false ;
};

#endif
