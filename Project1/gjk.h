#pragma once
#include <glm.hpp>
#include "Utils/General.h"
#include "Physics/Collision/Collider.h"
class gjk
{
public:
    bool intersect(Physics::Collider* coll1, Physics::Collider* coll2)
    {
        auto support = [&](glm::vec3& dir) {
            dir = glm::normalize(dir);
            return coll1->support(dir) - coll2->support(-dir);
        };
        v = glm::vec3(1, 0, 0); //some arbitrary starting vector

        c = support(v);
        if (dot(c, v) < 0)
            return false;

        v = -c;
        b = support(v);

        if (dot(b, v) < 0)
            return false;

        v = Utils::tripProduct(c - b, -b);
        n = 2;

        for (int iterations = 0; iterations < 32; iterations++)
        {
            glm::vec3 a = support(v);
            a_ = a;

            if (dot(a, v) < 0)
                return false;

            if (update(a)) {
                return true;
            }
        }

        //out of iterations, be conservative
        return true;
    }
    // retruns MTV
    glm::vec3 epa(Physics::Collider* coll1, Physics::Collider* coll2) {
        using edge = std::array<glm::vec3, 2>;
        using face = std::array<glm::vec3, 3>;
        using faces = std::list<face>;
        using edges = std::list<edge>;

        auto get_triangle_normal = [](const face& tri) -> glm::vec3 {
            auto t = cross(tri[1] - tri[0], tri[2] - tri[0]);
            if (t != Utils::zero())
                t = normalize(t);
            return t;
        };

        auto add_edge = [&](const edge& e, edges& es) -> bool {
            for (auto itt = es.begin(); itt != es.end();) {
                edge& e0 = *itt;
                if (e0[0] == e[1] && e0[1] == e[0]) {
                    es.erase(itt);
                    return false;
                }
            }
            es.push_back(e);
            return true;
        };

        auto get_closest = [&](faces& fs, float& dist) -> face* {
            dist = INFINITY;
            face* res = nullptr;
            for (face& f : fs) {
                float d = dot(f[0], get_triangle_normal(f));
                if (d < dist) {
                    dist = d;
                    res = &f;
                }
            }
            return res;
        };

        auto can_be_seen = [&](face& f, glm::vec3& v) -> bool {
            return dot(get_triangle_normal(f), v - f[0]);
        };

        faces fs;
        fs.push_back(face({ a_, b, c }));
        fs.push_back(face({ a_, c, d }));
        fs.push_back(face({ a_, d, b }));
        fs.push_back(face({ b, d, c }));
        edges es;
        for (unsigned counter = 0;; counter++) {
            es.clear();
            float dist = 0;
            face* ptr = get_closest(fs, dist);
            if (!ptr) {
                break;
            }
            face closest = *ptr;
            glm::vec3 face_normal = get_triangle_normal(closest);
            glm::vec3 v = coll1->support(face_normal) - coll2->support(-face_normal);

            if (dot(v, face_normal) - dist < 0.001f) {
                return face_normal * dist;
            }

            for (auto itt = fs.begin(); itt != fs.end();) {
                if (can_be_seen(*itt, v)) {
                    // add faces to loose edges
                    face& f = *itt;
                    for (char i = 0; i < 3; i++) {
                        add_edge(edge({f[i], f[(i + 1) % 3] }), es);
                    }
                    itt = fs.erase(itt);
                }
                else {
                    itt++;
                }
            }
            // fs.remove(closest);
            for (edge& e : es) {
                fs.push_back(face({ v, e[0], e[1] }));
                continue;
                /*
                fs.emplace_back(e[0], e[1], v);
                // fs[num_faces][0] = loose_edges[i][0];
                // fs[num_faces][1] = loose_edges[i][1];
                // fs[num_faces][2] = p;
                // fs[num_faces][3] = normalise(cross(loose_edges[i][0] - loose_edges[i][1], loose_edges[i][0] - p));

                //Check for wrong normal to maintain CCW winding
                float bias = 0.000001; //in case dot result is only slightly < 0 (because origin is on face)
                if (dot(fs.back()[0], get_triangle_normal(fs.back())) + bias < 0) {
                    glm::vec3 temp = fs.back()[0];
                    fs.back()[0] = fs.back()[1];
                    fs.back()[1] = temp;
                    fs.back()[3] = -faces[num_faces][3]; // the normal
                }
                */
            }
        }
        return Utils::zero();
    }
private:
    glm::vec3 v;
    glm::vec3 a_, b, c, d;
    unsigned int n;

    bool update(const glm::vec3& a) {
        glm::vec3 ao;

        glm::vec3 ab;
        glm::vec3 ac;
        glm::vec3 ad;

        glm::vec3 abc;
        glm::vec3 acd;
        glm::vec3 adb;
        switch (n)
        {
        case 0:
            b = a;
            v = -a;
            n = 1;
            return false;
        case 1:
            v = Utils::tripProduct(b - a, -a);
            c = b;
            b = a;
            n = 2;
            return false;
        case 2:
            ao = -a;

            ab = b - a;
            ac = c - a;

            abc = cross(ab, ac);

            const glm::vec3 abp = cross(ab, abc);

            if (dot(abp, ao) > 0) {
                c = b;
                b = a;
                v = Utils::tripProduct(ab, ao);
                return false;
            }

            const glm::vec3 acp = cross(abc, ac);
            if (dot(acp, ao) > 0) {
                b = a;
                v = Utils::tripProduct(ac, ao);
                return false;
            }

            if (dot(abc, ao) > 0) {
                d = c;
                c = b;
                b = a;

                v = abc;
            }
            else {
                d = b;
                b = a;
                v = -abc;
            }
            n = 3;
            return false;
        case 3:
            ao = -a;

            ab = b - a;
            ac = c - a;
            ad = d - a;

            abc = cross(ab, ac);
            acd = cross(ac, ad);
            adb = cross(ad, ab);
            
            glm::vec3 tmp;

            const int over_abc = 0x1;
            const int over_acd = 0x2;
            const int over_adb = 0x4;

            int plane_tests =
                (dot(abc, ao) > 0 ? over_abc : 0) |
                (dot(acd, ao) > 0 ? over_acd : 0) |
                (dot(adb, ao) > 0 ? over_adb : 0);

            switch (plane_tests)
            {
            case 0:
                //behind all three faces, thus inside the tetrahedron - we're done
                return true;

            case over_abc:
                goto check_one_face;

            case over_acd:
                //rotate ACD into ABC

                b = c;
                c = d;

                ab = ac;
                ac = ad;

                abc = acd;

                goto check_one_face;

            case over_adb:
                //rotate ADB into ABC

                c = b;
                b = d;

                ac = ab;
                ab = ad;

                abc = adb;

                goto check_one_face;

            case over_abc | over_acd:
                goto check_two_faces;

            case over_acd | over_adb:
                //rotate ACD, ADB into ABC, ACD

                tmp = b;
                b = c;
                c = d;
                d = tmp;

                tmp = ab;
                ab = ac;
                ac = ad;
                ad = tmp;

                abc = acd;
                acd = adb;

                goto check_two_faces;

            case over_adb | over_abc:
                //rotate ADB, ABC into ABC, ACD

                tmp = c;
                c = b;
                b = d;
                d = tmp;

                tmp = ac;
                ac = ab;
                ab = ad;
                ad = tmp;

                acd = abc;
                abc = adb;

                goto check_two_faces;

            default:
                //we've managed to build a horribly degenerate simplex
                //this could just as easily be an assert, but since this
                //code was originally used to reject definite non-intersections
                //as an optimization it conservatively returns true
                return true;
            }

        check_one_face:

            if (dot(cross(abc, ac), ao) > 0)
            {
                //in the region of AC

                b = a;

                v = Utils::tripProduct(ac, ao);

                n = 2;

                return false;
            }

        check_one_face_part_2:

            if (dot(cross(ab, abc), ao) > 0)
            {
                //in the region of edge AB

                c = b;
                b = a;

                v = Utils::tripProduct(ab, ao);

                n = 2;

                return false;
            }

            //in the region of ABC

            d = c;
            c = b;
            b = a;

            v = abc;

            n = 3;

            return false;

        check_two_faces:

            if (dot(cross(abc, ac), ao) > 0)
            {
                //the origin is beyond AC from ABC's
                //perspective, effectively excluding
                //ACD from consideration

                //we thus need test only ACD

                b = c;
                c = d;

                ab = ac;
                ac = ad;

                abc = acd;

                goto check_one_face;
            }

            //at this point we know we're either over
            //ABC or over AB - all that's left is the
            //second half of the one-face test

            goto check_one_face_part_2;
        }
    }
};