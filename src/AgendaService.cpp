#include "AgendaService.hpp"

#include <iostream>
/**
* constructor
*/
AgendaService::AgendaService() {}

/**
* destructor
*/
AgendaService::~AgendaService() {}

/**
* check if the username match password
* @param userName the username want to login
* @param password the password user enter
* @return if success, true will be returned
*/
bool userLogIn(const std::string userName, const std::string password) {
  filter = [=](const User &user)
      -> bool { return user.getName() == userName; } std::list<User> user_list =
             m_storage->queryUser(filter);
  if (user_list.empty())
    return false;
  if (user_list.front().getPassword == password)
    return true;
  return false;
}

/**
* regist a user
* @param userName new user's username
* @param password new user's password
* @param email new user's email
* @param phone new user's phone
* @return if success, true will be returned
*/
bool userRegister(const std::string userName, const std::string password,
                  const std::string email, const std::string phone) {
  filter = [=](const User &user)
      -> bool { return user.getName() == userName; } std::list<User> user_list =
             m_storage->queryUser(filter);
  if (!user_list.empty())
    return false;
  m_storage->createUser(userName, password, email, phone);
  return true;
}

/**
* delete a user
* @param userName user's username
* @param password user's password
* @return if success, true will be returned
*/
bool deleteUser(const std::string userName, const std::string password) {
  filter_1 = [=](const User &user) -> bool {
    return user.getName() == userName;
  } m_storage->deleteUser(filter_1);
  std::list<User> user_list = m_storage->queryUser(filter_1);
  filter_2 = [=](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName || meeting.isParticipator(userName);
  } m_storage->deleteMeeting(filter_2);
  return true;
}
/**
* list all users from storage
* @return a user list result
*/
std::list<User> listAllUsers(void) const {
  filter_1 = [=](const User &user)
      -> bool { return true; } std::list<User> list_all_users =
             queryMeeting(filter);
  return list_all_users;
}

/**
* create a meeting
* @param userName the sponsor's userName
* @param title the meeting's title
* @param participator the meeting's participator
* @param startData the meeting's start date
* @param endData the meeting's end date
* @return if success, true will be returned
*/
bool createMeeting(const std::string userName, const std::string title,
                   const std::string startDate, const std::string endDate,
                   const std::vector<std::string> participator) {
  filter_1 = [=](const User &user)
      -> bool { return user.getName() == userName; } std::list<User> user_list =
             m_storage->queryUser(filter_1);
  if (!user_list.empty())
    return false;
  filter_2 = [=](const Meeting &meeting)
      -> bool { return meeting.getTitle() == title; } std::list<Meeting>
             meeting_list = m_storage->queryMeeting(filter_2);
  if (!user_list.empty())
    return false;
  std::list<Meeting> sponsor_meeting_list = listAllMeetings(userName);
  if (!sponsor_meeting_list.empty()) {
    for (auto iterator = sponsor_meeting_list.begin();
         iterator != sponsor_meeting_list.end(); iterator++) {
      if (!(iterator->getStartDate() > endDate) &&
          !(iterator->getEndDate() < startDate)) {
        return false;
      }
    }
  }
  td::list<Meeting> participator_meeting_list;
  for (auto iterator_1 = participator.begin(); iterator_1 != participator.end();
       iterator_1++) {
    participator_meeting_list = listAllMeetings(*iterator_1);
    for (auto iterator_2 = participator_meeting_list.begin();
         iterator_2 != participator_meeting_list.end(); iterator_2++) {
      if (!(iterator_2->getStartDate() > endDate) &&
          !(iterator_2->getEndDate() < startDate)) {
        return false;
      }
    }
  }
  m_storage->createMeeting(userName, participator, startDate, endDate, title);
  return true;
}

/**
* search a meeting by username and title
* @param uesrName the sponsor's userName
1 * @param title the meeting's title
* @return a meeting list result
*/
std::list<Meeting> meetingQuery(const std::string userName,
                                const std::string title) const {
  filter = [=](const Meeting &meeting) -> bool {
    return (meeting.getName == userName && meeting.getTitle() == title);
  } std::list<Meeting>
                                              list_all_meetings =
                                                  queryMeeting(filter);
  return list_all_meetings;
}
/**
* search a meeting by username, time interval
* @param uesrName the sponsor's userName
* @param startDate time interval's start date
* @param endDate time interval's end date
* @return a meeting list result
*/
std::list<Meeting> meetingQuery(const std::string userName,
                                const std::string startDate,
                                const std::string endDate) const {
  filter = [=](const Meeting &meeting) -> bool {
    return (meeting.getName == userName && meeting.getStartDate == startDate &&
            meeting.getEndDate == endDate);
  } std::list<Meeting>
                                              list_all_meetings =
                                                  queryMeeting(filter);
  return list_all_meetings;
}

/**
* list all meetings the user take part in
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> listAllMeetings(const std::string userName) const {
  filter = [=](const Meeting &meeting) -> bool {
    return (meeting.getSponsor() == userName ||
            meeting.getParticipator == userName);
  } std::list<Meeting>
                                              list_all_meetings =
                                                  queryMeeting(filter);
  return list_all_meetings;
}

/**
* list all meetings the user sponsor
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting> listAllSponsorMeetings(const std::string userName) const {
  filter = [&](const User &user) -> bool {
    return (user.getSponsor() == userName);
  } std::list<Meeting>
                                        meeting_list =
                                            m_storage->queryMeeting(filter);
  return meeting_list;
}

/**
* list all meetings the user take part in and sponsor by other
* @param userName user's username
* @return a meeting list result
*/
std::list<Meeting>
listAllParticipateMeetings(const std::string userName) const {
  filter = [&](const User &user) -> bool {
    return (user.getParticipator() == userName);
  } std::list<Meeting>
                                        meeting_list =
                                            m_storage->queryMeeting(filter);
  return meeting_list;
}

/**
* delete a meeting by title and its sponsor
* @param userName sponsor's username
* @param title meeting's title
* @return if success, true will be returned
*/
bool deleteMeeting(const std::string userName, const std::string title) {
  filter = [&](const Meeting &meeting) -> bool {
    return (meeting.getSponsor() == userName && meeting.getTitle() == title);
  } int n = m_storage->deleteMeeting(filter);
  if (n == 0)
    return false;
  return true;
}

/**
* delete all meetings by sponsor
* @param userName sponsor's username
* @return if success, true will be returned
*/
bool deleteAllMeetings(const std::string userName) {
  filter_1 = [=](const User &user)
      -> bool { return user.getName() == userName; } std::list<User> user_list =
             m_storage->queryUser(filter_1);
  if (user_list.empty())
    return false;
  filter_2 = [=](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName;
  } m_storage->deleteMeeting(filter_2);
  return true;
}

/**
* start Agenda service and connect to storage
*/
void startAgenda(void) { m_storage = Storage::getInstance(); }

/**
* quit Agenda service
*/
void quitAgenda(void) { m_storage->sync(); }

#endif
