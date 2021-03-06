/***************************************************************************
 *   Copyright (C) 2008 by Łukasz Jernaś   *
 *   deejay1@srem.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "httpquery.h"
#include <QtDebug>

#include "MerkaartorPreferences.h"

namespace NameFinder
{

    HttpQuery::HttpQuery ( QObject *parent, QIODevice *device ) : QObject ( parent )
    {
        myService = M_PREFS->getNominatimUrl();
        myDevice = device;
    }
    HttpQuery::HttpQuery ( QObject *parent, QUrl service, QIODevice *device ) : QObject ( parent )
    {
        myService = service;
        myDevice = device;
    }


    HttpQuery::~HttpQuery()
    {
    }

    bool HttpQuery::startSearch ( QString question )
    {
        connect(&connection, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(on_responseHeaderReceived(const QHttpResponseHeader &)));
        connect(&connection,SIGNAL(requestFinished(int, bool)),this,SLOT(on_requestFinished(int, bool)));
        //if (!myService.isValid() || myService.scheme() != "http" || myService.path().isEmpty())
        //  return false;

        myService.addQueryItem ( "q",question );
        myService.addQueryItem ( "format","xml" );
        connection.setHost ( myService.host(), myService.port ( 80 ) );

        QHttpRequestHeader request( "GET", myService.path() + "?" + myService.encodedQuery() );
        if (myService.port(80) != 80)
            request.setValue( "Host", myService.host() + ":" + myService.port ( 80 ) );
        else
            request.setValue( "Host", myService.host() );
        request.setValue( "Connection", "Keep-Alive" );
        request.setValue("User-Agent", USER_AGENT);

        connection.setProxy(M_PREFS->getProxy(myService));
        reqId = connection.request( request, NULL, myDevice );
        connection.close();
        return true;
    }

    void HttpQuery::on_responseHeaderReceived(const QHttpResponseHeader & hdr)
    {
        switch (hdr.statusCode()) {
            case 200:
                break;
            default:
                qDebug() << hdr.statusCode();
                qDebug() << hdr.reasonPhrase();
                break;
        }
    }

    void HttpQuery::on_requestFinished ( int id, bool error )
    {
        if ((id == reqId) && !error) {
            emit done();
        }
        else if ((id == reqId) && error) {
            emit doneWithError(connection.error());
        }
    }
}
