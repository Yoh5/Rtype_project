#include "../scene/scene.hpp"
#include "../Ecs/system.hpp"
#include "../eventHandler/eventHandler.hpp"

/*Systems*/
//Each user-defined system will inherit from a class ASystem with start stop and update methods.

class ScoreSystem : public BJENGINE::ASystem {
    public:
    
    //you must provide the registry here exactly like this
    ScoreSystem(BJENGINE::registry &reg) : ASystem(reg){}

    void start() override
    {
        //initialize
    }

    void stop() override
    {
        //destroy
    }

    void update() override
    {
        //logic
    }
};

int main(void)
{
    /*--------------------Scene----------------------*/
    //A scene is a collection of entities and systems
    //MenuScene, Level1Scene, Level2Scene...
    //Each scene contains its instance of the registry

    BJENGINE::Scene scene1;
    auto &scene1_registry = scene1.get_registry();
    scene1_registry.register_component<int>();
    //etc

    while (true) {
        scene1.run();
    }
    //Note: You can switch between scenes. Example: switch from scene1 to scene2 by doing
    //scene1.setActive(false) : that will disable the scene1 run method


    /*---------------Systems handling--------------------------*/

    struct ScoreSysTag {};

    //registration
    scene1_registry.add_system<ScoreSysTag>(std::make_shared<ScoreSystem>(scene1_registry));

    //Note: Systems are run in order of registration in the scene run method(); so no need to run them yourself

    
    
    /*--------------Entities and Components------------*/
    int player_entt = scene1_registry.spawn_entity();

    scene1_registry.attach_component<int>(player_entt, 5);

    //Watch how to iterate over the components. Be careful about verfying validity of index to avoid segfaults
    auto &int_components = scene1_registry.get_components<int>();

    for (int i = 0; i < scene1_registry.entities(); i++)
    {
        //Don't try to use && statement, as it could crash. Always do these two verifications
        if (int_components.contains(i))
        {
            if (int_components[i].has_value())
            {
                //do someting

            }
        }
    }


    /*--------Event Handling-------------*/
    
    //Use this to handle events in your game / server or whatever you want

    BJENGINE::EventHandler evthandler;

    //register your events by passing a tag to the event, a trigger(function whcih return true if the event happens false otherwise),
    //and the callBack when the event is caught
    //Note that the eventHandler runs on a separate thread, just register properly the events and that is it

    //Example
    auto trigger = [](void) -> bool {
        return true;
    };

    auto callBack= [] {
        std::cout << "The callBack was called in response to the trigger. Oura!" << std::endl;
    };
    struct FakeTriggerTag{};

    evthandler.registerEvent<FakeTriggerTag>(trigger, callBack);

    return 0;
}