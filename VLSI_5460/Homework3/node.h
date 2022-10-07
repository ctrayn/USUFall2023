#pragma once
#include <string>
#include <vector>
#include <cmath>
using namespace std;

class pair_t {
    public:
        int width;
        int height;
        pair_t(int w, int h) {
            width = w;
            height = h;
        }
        pair_t() {
            width = 0;
            height = 0;
        }
};

class node_t {
    public:
        vector<pair_t> orientations;
        string id;

        node_t() {
            id = '\0';
            orientations = {};
        }
        node_t(string new_id) {
            id = new_id;
            orientations = {};
        }
        node_t(string new_id, float area, float aspect_ratio) {
            id = new_id;
            int height = sqrt(area / aspect_ratio);
            int width  = sqrt(area * aspect_ratio);
            set_orientations(height, width);
        }

        void set_orientations(int height, int width) {
            if (height == width) {
                pair_t orient = pair_t(width, height);
                orientations.push_back(orient);
            }
            else {
                pair_t orient = pair_t(width, height);
                orientations.push_back(orient);
                orient = pair_t(height, width);
                orientations.push_back(orient);
            }
        }

        void trim_orientations() {
            //TODO
            for (int i = 0; i < orientations.size() - 1; i++) {
                for (int j = i + 1; j < orientations.size(); j++) {
                    //case equal
                    if (orientations[i].height == orientations[j].height && orientations[i].width == orientations[j].width) {
                        orientations.erase(orientations.begin() + j);
                        j--;
                    }
                    //case heights are equal and widths are not
                    else if (orientations[i].height == orientations[j].height) {
                        if (orientations[i].width > orientations[j].width) {
                            orientations.erase(orientations.begin() + j);
                            j--;
                        }
                        else  { // orientations[i].width < orientations[j].width
                            orientations.erase(orientations.begin() + i);
                            i--;
                            break;  //i is gone, so don't compare the now dead value against anything else
                        }
                    }
                    //case widths are equal and widths are not
                    else if (orientations[i].width == orientations[j].width) {
                        if (orientations[i].height > orientations[j].height) {
                            orientations.erase(orientations.begin() + j);
                            j--;
                        }
                        else {  //orientations[i].height < orientations[j].height
                            orientations.erase(orientations.begin() + i);
                            i--;
                            break;
                        }
                    }
                    //case both orientations[i].height and orientations[i].width < orientations[j].height and orientations[j].width
                    else if (orientations[i].height < orientations[j].height && orientations[i].width < orientations[j].width) {
                        orientations.erase(orientations.begin() + j);
                        j--;
                    }
                    //case both orientations[j].height and orientations[j].width < orientations[i].height and orientations[i].width
                    else if (orientations[i].height > orientations[j].height && orientations[i].width > orientations[j].width) {
                        orientations.erase(orientations.begin() + i);
                        i--;
                        break;
                    }
                }
            }
        }
};