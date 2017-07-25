Feature: Digital Campaign Bundle

  @api @digital_campaign
  Scenario Outline: Access Embed Templates Overview Page.
    Given  CU - I am logged in as a user with the <role> role
      And am on "admin/content/embeds"
    Then I should see <message>

    Examples:
      | role            | message         |
      | content_editor  | "Add Embeds"    |
      | site_owner      | "Add Embeds"    |
      | administrator   | "Add Embeds"    |
      | developer       | "Add Embeds"    |
      | edit_my_content | "Access Denied" |
