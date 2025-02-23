#pragma once

#include "egm/egm-basic.h"
#include "optional"

namespace UML {
    struct BooleanDataPolicy : public EGM::AbstractDataPolicy {
        virtual bool getBool() = 0;
        virtual void setBool(bool b) = 0;
        virtual std::optional<bool> defaultBool() { return {}; }
        std::string getData() override {
            auto default_val = defaultBool();
            if ((!default_val.has_value() || !default_val.value()) && getBool()) {
                return "true";
            } else if ((!default_val.has_value() || default_val.value()) && !getBool()) {
                return "false";
            }
            return "";
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
