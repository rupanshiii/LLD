#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

/*
Question: 
Suppose a user can receive notifications through Email, SMS, and Push Notification. A user can have Email and SMS enabled at the same time, then enable Push Notification later, and disable any channel independently. Which relationship should we use?

Answer:

Composition is suitable because these notification channels are independent capabilities that can be combined.

It would be a poor design to create classes such as EmailUser, SMSUser, EmailSMSUser, EmailPushUser, and EmailSMSPushUser. The number of classes would keep growing as more notification channels and combinations are added.

Instead, the user or notification system can contain multiple notification-channel objects and use whichever channels are currently enabled.

There can still be a common NotificationChannel interface, with EmailNotification, SMSNotification, and PushNotification as its implementations. That part uses inheritance or interface polymorphism.

However, the relationship between the notification system and the channels is composition because multiple independent channels can be combined and changed independently.

The main clues are that multiple independent behaviors can exist together and those behaviors can be enabled, disabled, or changed independently.
*/


// INITIAL DESIGN - without considering the scalability

class NotificationSystem {
public:
    virtual void notify(string& s) = 0;
    virtual ~NotificationSystem() = default;
};

class Email : public NotificationSystem {
public:
    void notify (string& s) {
        cout<<"Email Notification: "<<s<<endl;
    }
};

class SMS : public NotificationSystem {
public:
    void notify (string& s) {
        cout<<"SMS Notification: "<<s<<endl;
    }
};

class Push : public NotificationSystem {
public:
    void notify (string& s) {
        cout<<"Push Notification: "<<s<<endl;
    }
};

class User {
private:
    NotificationSystem* email = new Email();
    NotificationSystem* sms = new SMS();
    NotificationSystem* push = new Push();
public:
    void newStock(){
        string s = "New stock available!";
        email->notify(s);
        sms->notify(s);
        push->notify(s);
    }
};


void solve(){
    User* user = new User();
    user->newStock(); cout<<endl;
}








// NEW DESIGN - SCALABLE
// improvements:
// 1. added const with the string notification passed to make the notif read only
// 2. earlier user was managing all the notifications, and add a new notification system forced user to update its code with the newest one. now user just maintain a list of notif systems, it would be managed by the main class
// 3. there was no functionality of how to enable / disable a notification, since its a common method, we added it in the base class itself
class NotificationSystem2 {
private:
    bool enabled = true;
public:
    virtual void notify(const string& s) = 0;
    // These functions do not need to be virtual because we are not overriding them
    void enable(){
        enabled = true;
    }
    void disable(){
        enabled = false;
    }
    bool getEnable(){
        return enabled;
    }
    virtual ~NotificationSystem2() = default;
};

class Email2 : public NotificationSystem2 {
public:
    void notify (const string& s) {
        cout<<"Email Notification: "<<s<<endl;
    }
};

class SMS2 : public NotificationSystem2 {
public:
    void notify (const string& s) {
        cout<<"SMS Notification: "<<s<<endl;
    }
};

class Push2 : public NotificationSystem2 {
public:
    void notify (const string& s) {
        cout<<"Push Notification: "<<s<<endl;
    }
};

class User2 {
private:
    set<NotificationSystem2*> notif;
public:
    void addNotif(NotificationSystem2* sys){
        notif.insert(sys);
    }
    void newStock(){
        string s = "New stock available!";
        for(auto x: notif){
            if(x->getEnable()){
                x->notify(s);
            }
        }
    }
};

void solve2(){
    User2* user = new User2();
    NotificationSystem2* email = new Email2();
    NotificationSystem2* push = new Push2();
    user->addNotif(email);
    user->addNotif(push);
    user->newStock(); cout<<endl;
    email->disable();
    user->newStock(); cout<<endl;
    NotificationSystem2* sms = new SMS2();
    user->addNotif(sms);
    user->newStock(); cout<<endl;
    email->enable();
    user->newStock(); cout<<endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll t = 1;
    // cin>>t;
    while(t--){
        solve();
        solve2();
    }

    return 0;
}