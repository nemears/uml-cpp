#pragma once

#include "egm/egm-basic.h"

namespace UML {
    struct BooleanDataPolicy : public EGM::AbstractDataPolicy {
        virtual bool getBool() = 0;
        virtual void setBool(bool b) = 0;
        std::string getData() override {
            if (getBool()) {
                return "true";
            } else {
                return "false";
            }
        }
        void setData(std::string data) override {
            if (data == "true") {
                setBool(true);
            } else if (data == "false") {
                setBool(false);
            } else if (data == "0") {
                setBool(false);
            } else if (data == "1") {
                setBool(true);
            }
        }
    };
}
