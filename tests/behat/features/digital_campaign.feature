@digital_campaign
Feature: Digital Campaign Bundle

  @embed_templates
  Scenario Outline: Access Embed Templates Overview Page.
    Given  I am logged in as a user with the <role> role
      And am on "admin/content/embeds"
    Then I should see <message>

    Examples:
      | role            | message         |
      | site_owner      | "Add Embeds"    |
      | administrator   | "Add Embeds"    |
      | developer       | "Add Embeds"    |
      | edit_my_content | "Access Denied" |
      | content_editor  | "Access Denied" |

  @embed_templates
  Scenario: Create a Facebook tracking pixel.
    Given  I am logged in as a user with the "site_owner" role
      And I am on "admin/content/embeds/add/facebook"
      And I fill in "Label" with "Facebook Tracking Pixel"
      And I fill in "Path" with "node/1"
      And I fill in "ID" with "123456"
      And I select "Published" from "Status"
    When I press "Save"
      And I am on "node/1"
    Then the response should contain "https://www.facebook.com/tr?id=123456&ev=PageView&noscript=1"

  @embed_templates
  Scenario: Create a Facebook tracking pixel.
    Given  I am logged in as a user with the "site_owner" role

  @embed_templates @javascript @broken
  # Turning the autocomplete field into a select list doesn't work like on my local...
  Scenario: Create An A/B test.
    Given  I am logged in as a user with the "site_owner" role
    When I am on "block/add/a-b-block"
      And I fill in "Label" with "A/B Block"
      And I select "Text Block A (Text Block A)" from "field_block_option[und][0][field_block][und]"
      And I fill in "edit-field-block-option-und-0-field-percentage-und-0-value" with "100"
      And I press "Add another item"
      And I wait 3 seconds
      And I select "Text Block B (Text Block B)" from "field_block_option[und][1][field_block][und]"
      And I fill in "edit-field-block-option-und-1-field-percentage-und-0-value" with "0"
    When I press "Save"
    Then I should see "A/B Block"
      And I should see "Text Block A Content AAA"
    When I follow "Edit Block"
      And I fill in "edit-field-block-option-und-0-field-percentage-und-0-value" with "0"
      And I fill in "edit-field-block-option-und-1-field-percentage-und-0-value" with "100"
    When I press "Save"
    Then I should see "A/B Block"
      And I should see "Text Block B Content BBB"
