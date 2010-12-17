#ifndef AppStateManager_h__
#define AppStateManager_h__

#include <vector>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "AppState.h"
#include "ObjectFactory/ObjectFactory.h"

namespace PZ
{
	typedef ::ObjectFactory<AppState*(), std::string> AppStateFactory;

	class AppStateManagerImpl;

	class AppStateManager
	{
	public:
		AppStateManager();
		~AppStateManager();

		void Update();

		template<class T>
		bool RegisterState(const std::string &stateName)
		{
			return getAppStateFactory().Register<T>(stateName);
		}
		bool UnregisterState(const std::string &stateName);
		//void AddState(const std::string &stateName, AppState *state);
		//void RemoveState(const std::string &stateName);
		//void DeleteAllStates();

		/*AppState *GetStateByName(const std::string &stateName);
		StateList GetAllStates();*/

		void ChangeState(const std::string &stateName, StringMap *const options = NULL);
		void PushState(const std::string &stateName, StringMap *const options = NULL);
		void PopState();
		void PopAllStack();

		bool IsEmpty();

		AppState *GetCurrentState();

	private:
		AppStateManagerImpl *p;

		AppStateFactory &getAppStateFactory();
	};
}

#endif // AppStateManager_h__