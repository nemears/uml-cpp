class FOO {};
class BAR {
    FOO f;
    //FOO f_array[3]; // TODO: so parsing this gives weird values (may be something with foo not being default constructible)
    FOO* f_ptr;
    FOO& f_ref;
};