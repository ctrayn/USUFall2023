#pragma once
#include <string>
#include <vector>
#include <cmath>
using namespace std;

class pair_t {
    public:
        float width;
        float height;
        pair_t(float w, float h) {
            width = w;
            height = h;
        }
        pair_t() {
            width = 0;
            height = 0;
        }
        void print(char end='\0') {
            printf("{Height %f Width %f}%c", height, width, end);
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
            float height = sqrt(area / aspect_ratio);
            float width  = sqrt(area * aspect_ratio);
            set_orientations(height, width);
            // printf("New node: ID %c Height %f Width %f\n", id[0], height, width);
        }

        void set_orientations(float height, float width) {
            pair_t orient;
            if (height == width) {
                orient = pair_t(width, height);
                orientations.push_back(orient);
            }
            else {
                orient = pair_t(width, height);
                orientations.push_back(orient);
                orient = pair_t(height, width);
                orientations.push_back(orient);
            }
            // printf("Adding orientation: ");
            // orient.print('\n');
        }

        void trim_orientations() {
            for (int i = 0; i < orientations.size() - 1; i++) {
                for (int j = i + 1; j < orientations.size(); j++) {
                    //case equal
                    if (orientations[i].height == orientations[j].height && orientations[i].width == orientations[j].width) {
                        orientations.erase(orientations.begin() + j);
                        j--;
                    }
                    //case heights are equal and widths are not
                    else if (orientations[i].height == orientations[j].height) {
                        if (orientations[i].width < orientations[j].width) {
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
                        if (orientations[i].height < orientations[j].height) {
                            orientations.erase(orientations.begin() + j);
                            j--;
                        }
                        else {  //orientations[i].height < orientations[j].height
                            orientations.erase(orientations.begin() + i);
                            i--;
                            break;
                        }
                    }
                    //case j is smaller
                    else if (orientations[i].height < orientations[j].height && orientations[i].width < orientations[j].width) {
                        orientations.erase(orientations.begin() + j);
                        j--;
                    }
                    //case i is smaller
                    else if (orientations[i].height > orientations[j].height && orientations[i].width > orientations[j].width) {
                        orientations.erase(orientations.begin() + i);
                        i--;
                        break;
                    }
                }
            }
        }
};