#pragma once
#include "base/ccConfig.h"
#if USE_PARTICLE > 0

#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"

extern se::Object* __jsb_cocos2d_ParticleSimulator_proto;
extern se::Class* __jsb_cocos2d_ParticleSimulator_class;

bool js_register_cocos2d_ParticleSimulator(se::Object* obj);
bool register_all_cocos2dx_particle(se::Object* obj);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_reset);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setGravity);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setEffect);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setPosVar);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_onDisable);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_bindNodeProxy);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setStartColorVar);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_emitParticle);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_stop);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_update);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setEndColorVar);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setSourcePos);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_active);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setEndColor);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setStopCallback);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setFinishedCallback);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_updateUVs);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_setStartColor);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_onEnable);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_getParticleCount);
SE_DECLARE_FUNC(js_cocos2dx_particle_ParticleSimulator_ParticleSimulator);

#endif //#if USE_PARTICLE > 0
