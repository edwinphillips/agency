<?php
/**
 * @file
 * Entties for Property
 *
 * This module sets up the entities for the module
 */

/**
 * Agency Import Mapping Entity implmenting Entity
 */
class AgencyImportMapping extends Entity {
  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'admin/agency/mapping/' . $this->identifier());
  }

}