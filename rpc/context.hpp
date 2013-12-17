#pragma once

#include <QObject>
#include <QStringList>
#include <QBuffer>
#include <qk/core/log.hpp>
#include <qk/core/formatter.hpp>
#include "session.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Rpc {

extern Log* rpclog(Log* pParent = 0);

class Context : public QObject
{
    Q_OBJECT

    friend class RpcRunnable;
    friend class Server;
    friend class Handler;

public:
    Context();
    virtual ~Context();

public:
    virtual QUuid sessionId() = 0;
    virtual QString path() = 0;

    void respondError(const Error& pError, quint32 pStatusCode = 500) { respondError(pError.toString(), pStatusCode); }
    void respondError(const QString& pError, quint32 pStatusCode = 500);

    Session* session() const { return mSession; }
    QString statusText() const;
    quint32 statusCode() const { return mStatusCode; }
    Log* log() const { return session() ? session()->log() : rpclog(); }
    Formatter* out() { ASSERTPTR(mOut); return mOut; }

protected:
    virtual void start();
    virtual void finish();
    void setSession(Session* pSession);

protected:
    QStringList mRespondErrors;
    quint32 mStatusCode = 200;
    QBuffer mResponseBuffer;

private:
    Session* mSession = nullptr;
    Formatter* mOut = nullptr;
};

}
}
