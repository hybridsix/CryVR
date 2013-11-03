/* Wiimote Controler node - for licensing and copyright see license.txt */

#include "StdAfx.h"
#include "Nodes/G2FlowBaseNode.h"
#include "Actor.h"

#include "Wiiuse/wiiuse.h" 
#include "Wiimote/CryVR_WiimoteManager.h"


class CryVR_WiimoteControler : public CFlowBaseNode<eNCT_Instanced>
{
public:
	bool active;
	SActivationInfo m_actInfo;
	int CryVR_WiimoteID;
	//imote** wiimotes;

	enum EInputPorts
	{
		EIP_IsServer = 0,
		EIP_WiimoteID = 1,
		EIP_MaxEvent = 2,
	};

	enum EOutputPorts
	{
		WIIMOTE_STATUS, //String
		WIIMOTE_A, //Pressed
		WIIMOTE_B, //Pressed
		WIIMOTE_UP, //Pressed
		WIIMOTE_DOWN, //Pressed
		WIIMOTE_LEFT, //Pressed
		WIIMOTE_RIGHT, //Pressed
		WIIMOTE_MINUS, //Pressed
		WIIMOTE_PLUS, //Pressed
		WIIMOTE_ONE, //Pressed
		WIIMOTE_TWO, //Pressed
		WIIMOTE_HOME, //Pressed
		WIIMOTE_RPY, //Vec3
		WIIMOTE_IR, //Vec3
		
		NUNCHUK_C,
		NUNCHUK_Z,
		NUNCHUK_RPY,
		NUNCHUK_JOYSTICK, //Vec3 z=0
		
	};


	////////////////////////////////////////////////////
	 CryVR_WiimoteControler(SActivationInfo *pActInfo) :
		m_actInfo()
	{
	}
	
	
	////////////////////////////////////////////////////
	virtual ~ CryVR_WiimoteControler(void){}

	////////////////////////////////////////////////////
	virtual void Serialize(SActivationInfo *pActInfo, TSerialize ser){}

	////////////////////////////////////////////////////
	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		// Define input ports here, in same order as EInputPorts
		static const SInputPortConfig inputs[] =
		{
			InputPortConfig<bool>("Activate", _HELP("Activate")),
			InputPortConfig<int>("WiimoteID", _HELP("Wiimote ID")),
			InputPortConfig<int>("MaxEvents", _HELP("Maximum events by frame")),
			{ 0 },
		};

		// Define output ports here, in same order as EOutputPorts
		static const SOutputPortConfig outputs[] =
		{
			OutputPortConfig<string>("Status", _HELP("")),
			
			OutputPortConfig<bool>("A", _HELP("")),
			OutputPortConfig<bool>("B", _HELP("")),
			OutputPortConfig<bool>("UP", _HELP("")),
			OutputPortConfig<bool>("DOWN", _HELP("")),
			OutputPortConfig<bool>("LEFT", _HELP("")),
			OutputPortConfig<bool>("RIGHT", _HELP("")),
			OutputPortConfig<bool>("MINUS", _HELP("")),
			OutputPortConfig<bool>("PLUS", _HELP("")),
			OutputPortConfig<bool>("ONE", _HELP("")),
			OutputPortConfig<bool>("TWO", _HELP("")),
			OutputPortConfig<bool>("HOME", _HELP("")),
			
			OutputPortConfig<Vec3>("WIIMOTE_RPY", _HELP("Wiimote Roll pitch yaw vec3")),
			OutputPortConfig<Vec3>("WIIMOTE_IR", _HELP("Wiimote IR vec3")),
			

			OutputPortConfig<bool>("C", _HELP("")),
			OutputPortConfig<bool>("Z", _HELP("")),
			OutputPortConfig<Vec3>("NUNCHUK_RPY", _HELP("Nunchuk Roll pitch yaw vec3")),
			OutputPortConfig<Vec3>("NUNCHUK_JOY", _HELP("Nunchuk Joystick vec3")),
			{0},
		};
		
		// Fill in configuration
		config.pInputPorts = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("CryVR node that retrieve Wiimote + Nunchuk values");
		config.SetCategory(EFLN_APPROVED);
	}

	


	////////////////////////////////////////////////////
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
	{
		
		if(event==eFE_Initialize) {
			active = GetPortBool(pActInfo, 0);
			pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID,true);
		}
		if(event==eFE_Activate){
			active = GetPortBool(pActInfo, 0);
		}
		if(event==eFE_Update  && active) {
			
			int w_id = GetPortInt(pActInfo, 1);
			int i=0;
			wiimote** manettes =  CryVR_WiimoteManager::wiimotes;
			
			//while (wiiuse_poll(manettes, CryVR_WiimoteManager::found) && i<3) {	
			while (wiiuse_poll(manettes, CryVR_WiimoteManager::found) && i<GetPortInt(pActInfo, 2)) {	
				
				if(CryVR_WiimoteManager::wiimotes[w_id]->event == WIIUSE_EVENT) {
					i++;
					handle_event(CryVR_WiimoteManager::wiimotes[w_id]);
				}
			}

			//CryLogAlways("Nombre d'events : %i" ,i);
		}

		
		switch (event)
		{
		case eFE_Initialize: 
			{
				m_actInfo = *pActInfo;
				break;
			}
		}
		
	}

	virtual void GetMemoryStatistics(ICrySizer *s){s->Add(*this);}
	virtual IFlowNodePtr Clone(SActivationInfo *pActInfo){return new  CryVR_WiimoteControler(pActInfo);}
	virtual void GetMemoryUsage(ICrySizer * s) const{s->Add(*this);}




	/**
 *	@brief Callback that handles an event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *
 *	This function is called automatically by the wiiuse library when an
 *	event occurs on the specified wiimote.
 */
	void handle_event(struct wiimote_t* wm) 
	{
		//CryLogAlways("\n\n--- EVENT [id %i] ---\n", wm->unid);
	
		/* if a button is pressed, report it */
	
		if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) ActivateOutput(&m_actInfo, WIIMOTE_A, true);// else ActivateOutput(&m_actInfo, WIIMOTE_A, false);
		if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)) ActivateOutput(&m_actInfo, WIIMOTE_B, true);// else ActivateOutput(&m_actInfo, WIIMOTE_A, false);
		if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN)) ActivateOutput(&m_actInfo, WIIMOTE_DOWN, true);// else ActivateOutput(&m_actInfo, WIIMOTE_A, false);
		if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT)) ActivateOutput(&m_actInfo, WIIMOTE_LEFT, true);// else ActivateOutput(&m_actInfo, WIIMOTE_A, false);
		if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT)) ActivateOutput(&m_actInfo, WIIMOTE_RIGHT, true);// else ActivateOutput(&m_actInfo, WIIMOTE_A, false);
		if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) ActivateOutput(&m_actInfo, WIIMOTE_MINUS, true);// else ActivateOutput(&m_actInfo, WIIMOTE_A, false);
		if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) ActivateOutput(&m_actInfo, WIIMOTE_PLUS, true);// else ActivateOutput(&m_actInfo, WIIMOTE_A, false);
	
	
		if (WIIUSE_USING_ACC(wm)) {
			Vec3 orient = Vec3(wm->orient.a_roll,wm->orient.a_pitch,wm->orient.yaw);
			ActivateOutput(&m_actInfo, WIIMOTE_RPY, orient);
		}
	
		/*
		if (WIIUSE_USING_IR(wm)) {
			int i = 0;

			for (; i < 4; ++i) {
				if (wm->ir.dot[i].visible) {
					CryLogAlways("IR source %i: (%u, %u)\n", i, wm->ir.dot[i].x, wm->ir.dot[i].y);
				}
			}

			//CryLogAlways("IR cursor: (%u, %u)\n", wm->ir.x, wm->ir.y);
			//CryLogAlways("IR z distance: %f\n", wm->ir.z);
		}
		*/
		/* show events specific to supported expansions */
	
		if (wm->exp.type == EXP_NUNCHUK)
		{
				struct nunchuk_t* nc = (nunchuk_t*)&wm->exp.nunchuk;

				if (IS_PRESSED(nc, NUNCHUK_BUTTON_C)) {
					ActivateOutput(&m_actInfo, NUNCHUK_C, true);
			
				}
				if (IS_PRESSED(nc, NUNCHUK_BUTTON_Z)) {
					ActivateOutput(&m_actInfo, NUNCHUK_Z, true);
			
				}

				//Optimisation necessaire ici !
				Vec3 orient = Vec3(nc->orient.roll,nc->orient.pitch,nc->orient.yaw);
				ActivateOutput(&m_actInfo, NUNCHUK_RPY, orient);
		
				Vec3 joy = Vec3( nc->js.ang,nc->js.mag,0);
				ActivateOutput(&m_actInfo, NUNCHUK_JOYSTICK, joy);
		} 
	
}
};

REGISTER_FLOW_NODE("CryVR:Controlers:Wii:WiimoteControlerTest",  CryVR_WiimoteControler);
