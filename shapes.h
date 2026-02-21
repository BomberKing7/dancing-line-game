#include "_3dvect.h"
#include <vector>
#include <string>
using namespace std;
struct point
{
    num x=0, y=0, z=0;
    point add(point b)
    {
        return point{x + b.x, y + b.y, z + b.z};
    }
    point sub(point b)
    {
        return point{x - b.x, y - b.y, z - b.z};
    }
    point onPlane()
    {
        return point{x, y, 0};
    }
};
struct poly
{
    vector<int> point_at;
    int colorIndex = 0;
    bool textured = 0;
    vector<int> tex_point_at;
};
struct object
{
    string name;
    int polyStart = 0, polyLen = 0;
    int pointStart = 0, pointLen = 0;
    vect shift = {0, 0, 0};
};
struct color
{
    string name = "";
    num r=1, g=1, b=1, a = 1;
};
struct fixed_animation
{
    int type = 0;
    fixed_animation(int k) { type = k; }
    bool operator==(int k) { return type == k; }
    bool operator!=(int k) { return type != k; }
};
struct animation
{
    int type=0;
    uint64_t lastTime = 0, end_time = -1;
    int target_object=0;
    vect rotation_vector;
    float rotation_angle=0;
    vect shift_vector;
    int speed_per_second=0;
    bool stop = 0;
};
animation moving(uint64_t last, float speed_per_second, vect shift, int target, uint64_t end_time = -1)
{
    cout << last << "  <<<< ";
    animation res;
    res.type = 0;
    res.speed_per_second = speed_per_second;
    res.shift_vector = shift;
    res.target_object = target;
    res.end_time = end_time;
    res.lastTime = last;
    cout << res.lastTime << "  <<<";
    return res;
}

struct ROAD
{
    int time=0;
    int ANI = 0;
    int direction = 0;
    float height = 0;
    int index = 0;
    int after=0, after1=0, before=0;
    float x=0, y=0;
    int lastTime = 0;
    // lastly set view
};
