// This file is part of IRC Chatter, the first IRC Client for MeeGo.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2011, Timur Kristóf <venemo@fedoraproject.org>

#ifndef UTIL_H
#define UTIL_H

#define GENPROPERTY(type, pname, name, settername, signalname) \
    private: type pname; \
    public: type name() const { return pname; } void settername (const type &value) { pname = value; emit signalname (); } \
    private:

#define GENPROPERTY_R(type, pname, name) \
    private: type pname; \
    public: type name() const { return pname; } \
    private:

#define GENPROPERTY_PTR(type, pname, name, settername, signalname) \
    private: type pname; \
    public: type name() { return pname; } void settername (type value) { pname = value; emit signalname (); } \
    private:

#endif // UTIL_H
