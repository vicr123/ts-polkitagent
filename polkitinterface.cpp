#include "polkitinterface.h"

PolkitInterface::PolkitInterface(QObject *parent) : PolkitQt1::Agent::Listener(parent)
{
    new PolkitAuthAgentAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerService("org.thesuite.polkitAuthAgent");

    if (!QDBusConnection::sessionBus().registerObject("/org/thesuite/polkitAuthAgent", this,
                                                      QDBusConnection::ExportScriptableSlots | QDBusConnection::ExportScriptableProperties | QDBusConnection::ExportAdaptors)) {
        qDebug() << "Could not initiate Authentication Agent on DBus!";
    }
    authWin = new Authenticate();
    connect(authWin, SIGNAL(okClicked()), this, SLOT(windowAccepted()));
    connect(authWin, SIGNAL(rejected()), this, SLOT(windowRejected()));
}

void PolkitInterface::windowAccepted() {
    this->session->setResponse(authWin->getPassword());
}

void PolkitInterface::windowRejected() {
    this->dialogCanceled = true;
    this->session->cancel();
}

void PolkitInterface::initiateAuthentication(const QString &actionId, const QString &message, const QString &iconName, const PolkitQt1::Details &details, const QString &cookie, const PolkitQt1::Identity::List &identities, PolkitQt1::Agent::AsyncResult *result) {
    currentIdentity = identities.first();
    this->asyncResult = result;
    this->cookie = cookie;
    authWin->setMessage(message);
    authWin->showFullScreen();

    this->initSession();
}

void PolkitInterface::initSession() {
    this->dialogCanceled = false;
    this->session = new PolkitQt1::Agent::Session(currentIdentity, cookie, asyncResult, this);
    connect(session, SIGNAL(request(QString,bool)), this, SLOT(sessionRequest(QString,bool)));
    connect(session, SIGNAL(completed(bool)), this, SLOT(sessionComplete(bool)));
    session->initiate();

    isAuthenticating = true;
}

void PolkitInterface::sessionComplete(bool ok) {
    this->authenticatedOk = ok;
    this->finish();
}

void PolkitInterface::finish() {
    if (authenticatedOk || dialogCanceled) {
        if (session == NULL) {
            this->asyncResult->setCompleted();
        } else {
            session->result()->setCompleted();
        }
        session->deleteLater();
        authWin->close();
        isAuthenticating = false;
    } else {
        session->deleteLater();
        initSession();
        authWin->showFullScreen(true);
    }
}

void PolkitInterface::sessionRequest(QString request, bool echo) {

}

bool PolkitInterface::initiateAuthenticationFinish() {
    return true;
}

void PolkitInterface::cancelAuthentication() {
    authWin->close();
    isAuthenticating = false;
}

void PolkitInterface::setWIdForAction(const QString &action, qulonglong wID) {

}
