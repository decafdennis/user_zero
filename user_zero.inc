<?php
// Developed by Dennis Stevense for Digital Deployment

function user_zero_fix() {
  user_zero_fix_anonymous_user(); 
}

function user_zero_fix_anonymous_user() {
  // Check whether the anonymous user exists
  $anonymous_user_exists = db_result(db_query("SELECT 1 FROM {users} WHERE uid = %d", USER_ZERO_ANONYMOUS_UID));
  
  if (!$anonymous_user_exists) {
    // Insert the anonymous user, the same way system_install() does it
    // TODO: Check for duplicate entries for name and mail?
    db_query("INSERT INTO {users} (name, mail) VALUES ('', '')");
    $inserted_uid = db_last_insert_id('users', 'uid');
    
    // Fix the uid of the record that has just been inserted
    if ($inserted_uid != USER_ZERO_ANONYMOUS_UID) {
      db_query("UPDATE {users} SET uid = %d WHERE uid = %d", USER_ZERO_ANONYMOUS_UID, $inserted_uid);
    }
    
    watchdog('user_zero', 'Recreated anonymous user', array(), WATCHDOG_WARNING);
  }
  
  // Ensure the anonymous user has the anonymous role
  db_query("INSERT IGNORE INTO {users_roles} (uid, rid) VALUES (%d, %d)", USER_ZERO_ANONYMOUS_UID, DRUPAL_ANONYMOUS_RID);
  
  if (db_affected_rows() > 0) {
    watchdog('user_zero', 'Reassigned anonymous role to anonymous user', array(), WATCHDOG_WARNING);
  }
}
