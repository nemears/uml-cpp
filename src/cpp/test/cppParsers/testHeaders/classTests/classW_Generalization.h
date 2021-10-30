class general {
    int member;
};

class other_general {};

class public_specific : public general {};

class protected_specific : protected general {};

class private_specific : private general {};

class unspecified_specific : general {};

class multiple_generals : public general, private other_general {};

class virtual_specific : virtual public public_specific, virtual private_specific {};