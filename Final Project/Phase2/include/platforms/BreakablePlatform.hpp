#include "platforms/Platform.hpp"
class Player;

class BreakablePlatform : public Platform {
public:
    BreakablePlatform(float x, float y);

    virtual void update(float dt) override;
    virtual bool onLandPlayer(Player& player) override;
    virtual void reset() override;
    bool isBreakable() const override { return true; }
private:

    // states
    bool isBroke;
    float fallSpeed;

};