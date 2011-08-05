#ifndef UTIL_H
#define UTIL_H

#define GENPROPERTY(type, pname, name, settername, signalname) \
    private: type pname; \
    public: type name() { return pname; } void settername (const type &value) { pname = value; emit signalname (); } \
    private:

#define GENPROPERTY_PTR(type, pname, name, settername, signalname) \
    private: type pname; \
    public: type name() { return pname; } void settername (type value) { pname = value; emit signalname (); } \
    private:

#endif // UTIL_H
