#include "General.h"


void Utils::log(const std::vector<std::string>& msgs, String eol) {
    std::string txt = "";
    for (String msg : msgs) {
        txt += msg;
    }
    std::cout << txt << eol;
}

void Utils::log(String msg, String eol) {
    std::cout << msg << eol;
}

bool Utils::inRange(Float value, Float target, Vector2 range) {
    return value > target - range.x AND value < target + range.y;
}

std::vector<std::string> Utils::split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
};

glm::mat4 Utils::translate(glm::mat4 matrix, Vector3 vec) {
    for (char i = 0; i < 3; i++)
        matrix[3][i] = vec[i];
    return matrix;
}

glm::mat4 Utils::rotate(Matrix4 matrix, Vector3 vec) {
    if (glm::all(glm::equal(vec, { 0, 0, 0 }))) return matrix;
    return glm::rotate(matrix, glm::radians(glm::length(vec)), glm::normalize(vec));
};

glm::mat4 Utils::rotate(Matrix4 matrix, Quaternion vec) {
    return matrix * glm::mat4_cast(vec);
}

bool Utils::contains(const std::string& a, const std::string& b) {
    const short s = b.size();
    for (short i = 0; i < a.size() - b.size(); i++) {
        const std::string& sub = a.substr(i, s);
        if (sub == b) {
            return true;
        }
    }
    return false;
}

bool Utils::contained(std::array<glm::vec2, 2> bound, glm::vec2 pos) {
    return glm::all(glm::greaterThanEqual(pos, bound[0]) && glm::lessThanEqual(pos, bound[1]));
}

bool Utils::contained(std::array<glm::vec3, 2> bound, glm::vec3 pos) {
    return glm::all(glm::greaterThanEqual(pos, bound[0]) && glm::lessThanEqual(pos, bound[1]));
}

std::string Utils::toUTF8(unsigned int codepoint)
{
    std::string out;

    if (codepoint <= 0x7f)
        out.append(1, static_cast<char>(codepoint));
    else if (codepoint <= 0x7ff)
    {
        out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff)
    {
        out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else
    {
        out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    return out;
}

glm::vec3 Utils::xAxis(float mag) {
    return glm::vec3(mag, 0, 0);
}

glm::vec3 Utils::yAxis(float mag) {
    return glm::vec3(0, mag, 0);
}

glm::vec3 Utils::zAxis(float mag) {
    return glm::vec3(0, 0, mag);
}

glm::vec3 Utils::zero() {
    return glm::vec3(0);
}

glm::vec3 Utils::fill(const float& num) {
    return glm::vec3(num);
}

const std::string Utils::getFileName(const std::string& filePath, const bool& includeExtension) {
    std::string dilimeter = "/";
    std::array<std::string, 2> possible = {
        "/", "\\"
    };
    for (const std::string& pos : possible) {
        if (contains(filePath, pos)) {
            dilimeter = pos;
            break;
        }
    }
    auto s = split(filePath, dilimeter);
    std::string res = s.back();
    if (!includeExtension) {
        s = split(res, ".");
        res = s[0];
    }
    return res;
}

glm::vec3 Utils::map(const glm::vec3& x, const glm::vec2& from, const glm::vec2& to) {
    float a = from.x;
    float b = from.y;
    float c = to.x;
    float d = to.y;
    return ((c + d) + (d - c) * ((2.0f * x - (a + b)) / (b - a))) / 2.0f;
}

float Utils::map(Float x, const glm::vec2& from, const glm::vec2& to) {
    float a = from.x;
    float b = from.y;
    float c = to.x;
    float d = to.y;
    return ((c + d) + (d - c) * ((2.0f * x - (a + b)) / (b - a))) / 2.0f;
}

/// <summary>
/// returns true if any are nan or inf regardles of sign
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
bool Utils::nan_inf(const glm::mat3& a) {
    for (char i = 0; i < 3; i++) {
        for (char j = 0; j < 3; j++) {
            if (glm::isnan(a[i][j]) || glm::isinf(a[i][j]) || glm::isinf(-a[i][j]))
                return true;
        }
    }
    return false;
}

glm::vec3 Utils::tripProduct(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    const float ca = glm::dot(c, a);
    const float cb = glm::dot(c, b);
    const glm::vec3 bca = b * ca;
    const glm::vec3 acb = a * cb;
    return b * glm::dot(a, c) - a * glm::dot(b, c);
}

// a x b x a
glm::vec3 Utils::tripProduct(const glm::vec3& a, const glm::vec3& b) {
    return tripProduct(a, b, a);
}

bool Utils::isParallel(const glm::vec3& a, const glm::vec3& b) {
    float d = fabsf(glm::dot(a, b));
    float t = glm::length(a) * glm::length(b);
    return d == t;
}

bool Utils::isPerpendicular(const glm::vec3& a, glm::vec3& b) {
    return !glm::dot(a, b);
}

glm::vec3 Utils::linePlaneIntersection(const glm::vec3& rayVector, const glm::vec3& rayPoint, const glm::vec3& planeNormal, const glm::vec3& planePoint) {
    const glm::vec3 diff = rayPoint - planePoint;
    const float prod1 = glm::dot(diff, planeNormal);
    const float prod2 = glm::dot(rayVector, planeNormal);
    const float prod3 = prod1 / prod2;
    return rayPoint - rayVector * prod3;
}

float Utils::isInFront(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& planeNormal) {
    return glm::dot(glm::normalize(planeNormal), p1 - p2);
}

glm::vec3 Utils::projectPointPlane(const glm::vec3& n, const glm::vec3& o, const glm::vec3& p) {
    return p - (glm::dot(n, p - o)) * n;
}

glm::mat3 Utils::inverse(const glm::mat3& a, const bool symetrical) {
    if (symetrical) {
        glm::mat3 res(1.0f);
        for (char i = 0; i < 3; i++)
            res[i][i] /= a[i][i];
        return res;
    }
    else
        return glm::inverse(a);
}

float Utils::round(float var, int dp)
{
    float t = powf(10, dp);
    // 37.66666 * 100 =3766.66 
    // 3766.66 + .5 =3767.16    for rounding off value 
    // then type cast to int so value is 3767 
    // then divided by 100 so the value converted into 37.67 
    float value = static_cast<int>(var * t + .5);
    return static_cast<float>(value / t);
}

// doenst go deep
int Utils::countFiles(String dirPath) {
    auto dirIter = std::filesystem::directory_iterator(dirPath);
    int fileCount = 0;

    for (auto& entry : dirIter)
    {
        if (entry.is_regular_file())
        {
            ++fileCount;
        }
    }
    return fileCount;
}

glm::vec3 Utils::normalize(Vector3 a) {
    glm::vec3 res = glm::normalize(a);
    for (char i = 0; i < 3; i++) {
        float& x = res[i];
        if (glm::isnan(x) OR glm::isinf(x)) {
            x = 0;
        }
    }
    return res;
}

/// <summary>
/// returns the matrix but without nan or inf
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
glm::mat4 Utils::validate(glm::mat4 a, float val) {
    for (char i = 0; i < 4; i++) {
        for (char j = 0; j < 4; j++) {
            float& element = a[i][j];
            if (glm::isnan(element) OR glm::isinf(element)) {
                element = val;
            }
        }
    }
    return a;
}

/// <summary>
/// replaces all occorances in the string
/// </summary>
std::string Utils::replaceAll(std::string str, std::string from, std::string to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::string Utils::to_string_precision(Vector3 vec, Int dp) {
    std::string res = "";
    for (char i = 0; i < 3; i++) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(dp) << vec[i];
        res += stream.str() + ", ";
        stream.clear();
    }
    res.pop_back();
    res.pop_back();
    return res;
}

float Utils::random(float min, float max)
{
    assert(min >= 0);
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max - min) + min;
}



Utils::Timer::Timer() : s(), e(), pausing()
{
    name = "Timer misc";
};
Utils::Timer::Timer(const std::string name) : s(), e(), pausing(), name(name) {

}
void Utils::Timer::start() { s = getNow(); }
void Utils::Timer::start(String name)
{
    reName(name);
    start();
}
;
void Utils::Timer::stop()
{
    e = getNow();
    if (pausing.size() % 2 != 0)
        pausing.push_back(e);
};
bool Utils::Timer::pause()
{
    if (pausing.size() % 2 == 0) {
        pausing.push_back(getNow());
        return true;
    }
    return false;
};
bool Utils::Timer::resume()
{
    if (pausing.size() % 2 != 0) {
        pausing.push_back(getNow());
        return true;
    }
    return false;
};
void Utils::Timer::log(bool frames) {
    stop();
    std::string units = frames ? " Frames" : " Milliseconds";
    std::cout << name << ": " << std::to_string(getDuration(frames)) << units << std::endl;
};
const float& Utils::Timer::getDuration(bool frames)
{
    float d = calcDurration(s, e) / 1000.0f;
    assert(!(pausing.size() % 2) && "Timer pausing not working");
    for (short i = 0; i < pausing.size(); i += 2)
        d -= calcDurration(elementAt(pausing, i), elementAt(pausing, i + 1)) / 1000.0f;
    if (frames)
        d = (d / 1000.0f) * 60.0f;
    return d;
};
void Utils::Timer::reName(String name) {
    this->name = name;
}
const std::chrono::time_point<std::chrono::high_resolution_clock> Utils::Timer::getNow()
{ 
    return std::chrono::high_resolution_clock::now(); 
};
float Utils::Timer::calcDurration(const std::chrono::time_point<std::chrono::high_resolution_clock>& s, const std::chrono::time_point<std::chrono::high_resolution_clock>& e) {
    return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count();
}


const Utils::BigMaths::Vector12 Utils::BigMaths::combine(const Utils::BigMaths::Vector6& a, const Utils::BigMaths::Vector6& b)
{
    Vector12 res{};
    for (char i = 0; i < 6; i++) {
        res[i] = a[i];
        res[i + 6] = b[i];
    }
    // res[0] = a[0];
    // res[1] = a[1];
    // res[2] = a[2];
    // res[3] = a[3];
    // res[4] = a[4];
    // res[5] = a[5];
    // 
    // res[6] =  b[0];
    // res[7] =  b[1];
    // res[8] =  b[2];
    // res[9] =  b[3];
    // res[10] = b[4];
    // res[11] = b[5];
    return res;
}

const Utils::BigMaths::Matrix12 Utils::BigMaths::combine(const Utils::BigMaths::MassMatrix6& a, const Utils::BigMaths::MassMatrix6& b)
{
    Matrix12 res{};
    for (char i = 0; i < 6; i++) {
        for (char j = 0; j < 6; j++) {
            res[i][j] = a[i][j];
            res[i + 6][j + 6] = b[i][j];
        }
    }
    return res;
}
// 1/n element wise
const Utils::BigMaths::Matrix12 Utils::BigMaths::inverse(const Utils::BigMaths::Matrix12& m)
{
    Matrix12 res{};
    for (char i = 0; i < 12; i++) {
        for (char j = 0; j < 12; j++) {
            if (m[i][j] == 0) {
                res[i][j] = 0;
            }
            else {
                res[i][j] = 1.0f / m[i][j];
            }
        }
    }
    return res;
}

std::array<Utils::BigMaths::Vector6, 2> Utils::BigMaths::split(const Utils::BigMaths::Vector12& a) {
    Vector6 r1{}, r2{};
    for (char i = 0; i < 6; i++) {
        r1[i] = a[i];
        r2[i] = a[i + 6];
    }
    return { r1, r2 };
}
std::array<glm::vec3, 2> Utils::BigMaths::split(const Utils::BigMaths::Vector6& a) {
    glm::vec3 r1{}, r2{};
    for (char i = 0; i < 3; i++) {
        r1[i] = a[i];
        r2[i] = a[i + 3];
    }
    return { r1, r2 };
}


// dot product [1 x 1]
float operator * (const Utils::BigMaths::Vector12& a, const Utils::BigMaths::Vector12& b)
{
    float res = 0;
    for (char i = 0; i < 12; i++)
        res += a[i] * b[i];
    return res;
}
// element wise multiplication
Utils::BigMaths::Vector12 operator * (const Utils::BigMaths::Vector12& a, const float& b)
{
    Utils::BigMaths::Vector12 res = a;
    for (char i = 0; i < 12; i++)
        res[i] *= b;
    return res;
}
// dot product [12 x 1]
Utils::BigMaths::Vector12 operator * (const Utils::BigMaths::Matrix12& a, const Utils::BigMaths::Vector12& b)
{
    Utils::BigMaths::Vector12 res{};
    for (char i = 0; i < 12; i++)
        for (char j = 0; j < 12; j++)
            res[i] += a[i][j] * b[i];
    return res;
}
// dot product [1 x 12]
Utils::BigMaths::Vector12 operator * (const Utils::BigMaths::Vector12& a, const Utils::BigMaths::Matrix12& b)
{
    Utils::BigMaths::Vector12 res{};
    for (char i = 0; i < 12; i++)
        for (char j = 0; j < 12; j++)
            res[i] += a[j] * b[j][i];
    return res;
}
// dot product [3 x 1]
glm::vec3 operator * (const glm::mat3& a, const glm::vec3& b)
{
    glm::vec3 res(0);
    for (char i = 0; i < 3; i++) {
        for (char j = 0; j < 3; j++) {
            res[i] += a[i][j] * b[i];
        }
    }
    return res;
}
// element wise addition
const Utils::BigMaths::Vector12 operator +(const Utils::BigMaths::Vector12& a, const Utils::BigMaths::Vector12& b)
{
    Utils::BigMaths::Vector12 res{};
    for (char i = 0; i < 12; i++)
        res[i] = a[i] + b[i];
    return res;
}
// element wise division
const Utils::BigMaths::Vector12 operator /(const Utils::BigMaths::Vector12& a, const float& b) {
    return a * (1.0f / b);
}

