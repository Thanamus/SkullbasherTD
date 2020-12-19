

class PathFinder(GameObject* gameObject)
 : Component(gameObject)
{
public:
    PathFinder(/* args */);
    ~PathFinder();

private:
    /* data */
    glm::vec3 nextPathPoint;
};

