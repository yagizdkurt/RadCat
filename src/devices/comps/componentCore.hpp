#define COMPONENT template<typename... Components> class BaseDevice; class EmptyDevice;

class EmptyDevice;
class BaseComponent {
public:
    virtual ~BaseComponent() = default;
    virtual void update(){}
    virtual void initialize(){}
    EmptyDevice* parent;
};