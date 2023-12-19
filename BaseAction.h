#pragma once

class Character;

class BaseAction {
protected:
    Character* pCharacter_;

public:
    BaseAction(Character* obj);
    virtual ~BaseAction() {};
    virtual void Update() = 0;
    virtual void Initialize() {}
    virtual void Terminate() {}
};