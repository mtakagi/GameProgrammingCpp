#pragma once

class AIComponent;

class AIState {
public:
    explicit AIState(AIComponent* owner) : mOwner(owner) {};
    virtual ~AIState() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    [[nodiscard]]
    virtual const char* GetName() const = 0;

protected:
    AIComponent* mOwner;
};

class AIPatrol : public AIState {
public:
    explicit AIPatrol(AIComponent* owner) : AIState(owner) {}

    void Update(float deltaTime) override;
    void OnEnter() override;
    void OnExit() override;

    [[nodiscard]]
    const char* GetName() const override { return "Patrol"; }
};

class AIDeath : public AIState {
public:
    explicit AIDeath(AIComponent* owner) : AIState(owner) {}

    void Update(float deltaTime) override;
    void OnEnter() override;
    void OnExit() override;

    [[nodiscard]]
    const char* GetName() const override { return "Death"; }
};

class AIAttack : public AIState {
public:
    explicit AIAttack(AIComponent* owner) : AIState(owner) {}

    void Update(float deltaTime) override;
    void OnEnter() override;
    void OnExit() override;

    [[nodiscard]]
    const char* GetName() const override { return "Attack"; }
};