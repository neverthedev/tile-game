# Class Diagram - Raylib Game Project

# Graphics component
This is component structure of game object
```plantuml
  @startuml game object component
    left to right direction
    skinparam linetype curved

    abstract class GameObject #wheat {
      + <b>constructor</b> GameObject(\n\tInputComponent&,
        \tUpdateComponent&, \n\tGraphicsComponent&)
      + {abstract} HandleInput()
      + {abstract} Update()
      + {abstract} Render(Graphics&)
    }

    together {
      abstract class InputComponent #wheat {
        + <b>constructor</b> InputComponent()
        + {abstract} void HandleInput(GameObject&)
      }

      abstract class UpdateComponent #wheat {
        + <b>constructor</b> UpdateComponent()
        + {abstract} void Update(GameObject&)
      }

      abstract class GraphicsComponent #wheat {
        + <b>constructor</b> GraphicsComponent()
        + {abstract} void Render(GameObject&, Graphics&)
      }

      GameObject *-- InputComponent
      GameObject *-- UpdateComponent
      GameObject *-- GraphicsComponent
      GameObject::HandleInput ..> InputComponent::HandleInput: >
      GameObject::Update ..> UpdateComponent::Update: >
      GameObject::Render ..> GraphicsComponent::Render: >
    }

    package input {
      class WorldInputComponent {
        + void HandleInput(GameObject&)
      }
      InputComponent <|-- WorldInputComponent
    }

    package update {
      class WorldUpdateComponent {
        + void Update(GameObject&)
      }
      UpdateComponent <|-- WorldUpdateComponent
    }

    package graphics {
      class WorldGraphicsComponent {
        + void Render(GameObject&, Graphics&)
      }
      GraphicsComponent <|-- WorldGraphicsComponent
    }

    GameWorld -|> GameObject
    class GameWorld {
      + <b>constructor</b> GameWorld(\n\tint width, int height, \n\tTileManager&)
    }

    note top of GameWorld
      constructor with all the components
      that parent class has
    end note

    GameWorld --- WorldInputComponent: dynamic_cast<GameWorld*>
    GameWorld --- WorldUpdateComponent
    GameWorld --- WorldGraphicsComponent
  @enduml
```

# General data

This diagram shows the class structure and relationships in the raylib-my project!
```plantuml
@startuml class_diagram
skinparam pageMargin 10
skinparam pageExternalColor gray
skinparam pageBorderColor black

hide *

!include classes.iuml

show GameObject
show Menu
show GameWorld
show GameArea
show GameCamera
show WorldTile
show WorldTileTerrainType
show DecorationMenu
show TilesManager

hide protected members
hide private members

remove @unlinked
@enduml
```
