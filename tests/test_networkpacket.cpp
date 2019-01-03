/*
 * Copyright 2013 Albert Vaca <albertvaka@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * Copyright 2018 Richard Liebscher <richard.liebscher@gmail.com>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <test.h>

#include <sailfishconnect/networkpackage.h>
#include <sailfishconnect/networkpackagetypes.h>

using namespace SailfishConnect;


TEST(NetworkPacketTests, networkPacketTest) {
    NetworkPackage np(QStringLiteral("com.test"));

    np.set(QStringLiteral("hello"),"hola");
    EXPECT_EQ(np.get<QString>("hello", "bye") , QString("hola"));

    np.set(QStringLiteral("hello"),"");
    EXPECT_EQ(np.get<QString>("hello", "bye") , QString(""));

    np.body().remove(QStringLiteral("hello"));
    EXPECT_EQ(np.get<QString>("hello", "bye") , QString("bye"));

    np.set(QStringLiteral("foo"), "bar");
    QByteArray ba = np.serialize();
    //qDebug() << "Serialized packet:" << ba;
    NetworkPackage np2(QLatin1String(""));
    NetworkPackage::unserialize(ba, &np2);

    EXPECT_EQ(np.id(), np2.id());
    EXPECT_EQ(np.type(), np2.type());
    EXPECT_EQ(np.body(), np2.body());

    QByteArray json("{\"id\":\"123\",\"type\":\"test\",\"body\":{\"testing\":true}}");
    //qDebug() << json;
    NetworkPackage::unserialize(json, &np2);
    EXPECT_EQ(np2.id(), QString("123"));
    EXPECT_EQ((np2.get<bool>("testing")), true);
    EXPECT_EQ((np2.get<bool>("not_testing")), false);
    EXPECT_EQ((np2.get<bool>("not_testing", true)), true);
}

TEST(NetworkPacketTests, DISABLED_networkPacketIdentityTest) {
    NetworkPackage np(QLatin1String(""));
    NetworkPackage::createIdentityPackage(&np);  // fix dependency injuection ;)

    EXPECT_EQ(np.get<int>("protocolVersion", -1),
              NetworkPackage::s_protocolVersion);
    EXPECT_EQ(np.type(), PACKAGE_TYPE_IDENTITY);
}