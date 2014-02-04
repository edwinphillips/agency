<?php
/**
 * @file
 * Entties for Property
 *
 * This module sets up the entities for the module
 */

/**
 * Property Entity implmenting Entity
 */
class AgencyProperty extends Entity {
  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'admin/agency/property/' . $this->identifier());
  }

}

/**
 * Media Entity implmenting Entity
 */
class AgencyMedia extends Entity {
  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'admin/agency/media/' . $this->identifier());
  }

}

/**
 * Categorisation Entity implmenting Entity
 */
class AgencyCategorisation extends Entity {
  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'admin/agency/categorisation/' . $this->identifier());
  }

}

/**
 * Feature Entity implmenting Entity
 */
class AgencyFeature extends Entity {
  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'admin/agency/feature/' . $this->identifier());
  }

}
