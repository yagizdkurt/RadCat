



class BaseComponent {
public:
    virtual ~BaseComponent() = default;
    virtual void update(){}
    virtual void initialize(){}
};