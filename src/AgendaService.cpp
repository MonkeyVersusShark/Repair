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
bool AgendaService::userLogIn(const std::string userName,
                              const std::string password) {
  auto filter = [&](const User &user) -> bool {
    return user.getName() == userName;
  };
  std::list<User> user_list = m_storage->queryUser(filter);
  if (user_list.empty())
    return false;
  if (user_list.front().getPassword() == password)
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
bool AgendaService::userRegister(const std::string userName,
                                 const std::string password,
                                 const std::string email,
                                 const std::string phone) {
  auto filter = [&](const User &user) -> bool {
    return user.getName() == userName;
  };
  std::list<User> user_list = m_storage->queryUser(filter);
  if (!user_list.empty())
    return false;
  User user(userName, password, email, phone);
  m_storage->createUser(user);
  return true;
}

/**
 * delete a user
 * @param userName user's username
 * @param password user's password
 * @return if success, true will be returned
 */
bool AgendaService::deleteUser(const std::string userName,
                               const std::string password) {
  auto filter_1 = [&](const User &user) -> bool {
    return user.getName() == userName;
  };
  m_storage->deleteUser(filter_1);
  std::list<User> user_list = m_storage->queryUser(filter_1);
  auto filter_2 = [=](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName || meeting.isParticipator(userName);
  };
  m_storage->deleteMeeting(filter_2);
  return true;
}
/**
 * list all users from storage
 * @return a user list result
 */
std::list<User> AgendaService::listAllUsers(void) const {
  auto filter = [&](const User &user) -> bool { return true; };
  std::list<User> list_all_users = m_storage->queryUser(filter);
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
bool AgendaService::createMeeting(const std::string userName,
                                  const std::string title,
                                  const std::string startDate,
                                  const std::string endDate,
                                  const std::vector<std::string> participator) {
  auto filter_1 = [&](const User &user) -> bool {
    return user.getName() == userName;
  };
  std::list<User> user_list = m_storage->queryUser(filter_1);
  if (!user_list.empty())
    return false;
  auto filter_2 = [&](const Meeting &meeting) -> bool {
    return meeting.getTitle() == title;
  };
  std::list<Meeting> meeting_list = m_storage->queryMeeting(filter_2);
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
  std::list<Meeting> participator_meeting_list;
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
  m_storage->createMeeting(
      Meeting(userName, participator, startDate, endDate, title));
  return true;
}

/**
 * search a meeting by username and title
 * @param uesrName the sponsor's userName
1 * @param title the meeting's title
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                               const std::string title) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return (meeting.getSponsor() == userName && meeting.getTitle() == title);
  };
  std::list<Meeting> list_all_meetings = m_storage->queryMeeting(filter);
  return list_all_meetings;
}
/**
 * search a meeting by username, time interval
 * @param uesrName the sponsor's userName
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
std::list<Meeting>
AgendaService::meetingQuery(const std::string userName,
                            const std::string startDate,
                            const std::string endDate) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return (meeting.getSponsor() == userName &&
            meeting.getStartDate() == startDate &&
            meeting.getEndDate() == endDate);
  };
  std::list<Meeting> list_all_meetings = m_storage->queryMeeting(filter);
  return list_all_meetings;
}

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting>
AgendaService::listAllMeetings(const std::string userName) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return (meeting.getSponsor() == userName ||
            meeting.isParticipator(userName));
  };
  std::list<Meeting> list_all_meetings = m_storage->queryMeeting(filter);
  return list_all_meetings;
}

/**
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting>
AgendaService::listAllSponsorMeetings(const std::string userName) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return (meeting.getSponsor() == userName);
  };
  std::list<Meeting> meeting_list = m_storage->queryMeeting(filter);
  return meeting_list;
}

/**
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting>
AgendaService::listAllParticipateMeetings(const std::string userName) const {
  auto filter = [&](const Meeting &meeting) -> bool {
    return (meeting.isParticipator(userName));
  };
  std::list<Meeting> meeting_list = m_storage->queryMeeting(filter);
  return meeting_list;
}

/**
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const std::string userName,
                                  const std::string title) {
  auto filter = [&](const Meeting &meeting) -> bool {
    return (meeting.getSponsor() == userName && meeting.getTitle() == title);
  };
  int n = m_storage->deleteMeeting(filter);
  if (n == 0)
    return false;
  return true;
}

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string userName) {
  auto filter_1 = [&](const User &user) -> bool {
    return user.getName() == userName;
  };
  std::list<User> user_list = m_storage->queryUser(filter_1);
  if (user_list.empty())
    return false;
  auto filter_2 = [&](const Meeting &meeting) -> bool {
    return meeting.getSponsor() == userName;
  };
  m_storage->deleteMeeting(filter_2);
  return true;
}

/**
 * start Agenda service and connect to storage
 */
void AgendaService::startAgenda(void) { m_storage = Storage::getInstance(); }

/**
 * quit Agenda service
 */
void AgendaService::quitAgenda(void) { m_storage->sync(); }
