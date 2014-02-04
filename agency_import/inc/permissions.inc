<?php

/**
 * Callback function to check user has permission.
 *
 * @param string $op
 *   The operation
 * @param object $entity
 *   The entity you are checking
 * @param object $account
 *   The user account to check against
 *
 * @return bool
 *   User does or doesn't have permission
 */
function agency_import_access_callback($op, $entity = NULL, $account = NULL) {
  if (user_access('administer agency', $account)) {
    return TRUE;
  }
  return FALSE;
}