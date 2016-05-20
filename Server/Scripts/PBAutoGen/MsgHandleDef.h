class CSLoginHandler : public MessageHandler {
public:
	bool Handler(Session &session, Message *msg);
};
class SCLoginRetHandler : public MessageHandler {
public:
	bool Handler(Session &session, Message *msg);
};
