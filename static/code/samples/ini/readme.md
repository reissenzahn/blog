
https://github.com/benhoyt/inih/blob/master/ini.h

# INI file

- An INI file is a configuration file that consists of a text-based content with a structure and syntax comprising key-value pairs for properties and sections that organize the properties.
- INI is an informal format with features that vary from parser to parser.


```ini
; last modified 1 April 2001 by John Doe
[owner]
name = John Doe
organization = Acme Widgets Inc.

[database]
; use IP address in case network name resolution is not working
server = 192.0.2.62     
port = 143
file = "payroll.dat"
```


## Keys

- The basic element contained in an INI file is the key. Every key has a name and a value, delimited by an equals sign (=).
- In the Windows implementation the equal sign and the semicolon are reserved characters and cannot appear in the key. The value can contain any character.
- Optional "global" properties may also be allowed, that are declared before any section is declared.
- Leading and trailing whitespaces around the outside of the property name are ignored.

```ini
name = value
```

## Sections

0 Keys may be grouped into arbitrarily named sections.
- The section name appears on a line by itself, in square brackets and all keys after the section declaration are associated with that section.
- Sections end at the next section declaration, or at the end of the file.

[section]
key1 = a
key2 = b
Case sensitivity
Section and property names are not case sensitive in the Windows implementation,[9] but other applications may behave differently.

The order of properties in a section and the order of sections in a file is irrelevant.


## Comments

- Semicolons (;) at the beginning of the line indicate a comment. Comment lines are ignored.

```ini
; comment text
```



## Section Nesting

- Some parsers allow section nesting, using dots as path delimiters:
- In some cases relative nesting is supported too, where a leading dot expresses nesting to the previous section.

```ini
[section]
domain = wikipedia.org

[section.subsection]
foo = bar
```

```ini
[section]
domain = wikipedia.org

[.subsection]
foo = bar
```


Duplicate names
Most implementations only support having one property with a given name in a section. The second occurrence of a property name may cause an abort, it may be ignored (and the value discarded), or it may override the first occurrence (with the first value discarded). Some programs use duplicate property names to implement multi-valued properties.

Interpretation of multiple section declarations with the same name also varies. In some implementations, duplicate sections simply merge their properties, as if they occurred contiguously. Others may abort, or ignore some aspect of the INI file.

Quoted values
Some implementations allow values to be quoted, typically using double quotes and/or apostrophes. This allows for explicit declaration of whitespace, and/or for quoting of special characters (equals, semicolon, etc.). The standard Windows function GetPrivateProfileString supports this, and will remove quotation marks that surround the values.

Escape characters
Some implementations offer varying support for an escape character, typically with the backslash (\) following the C syntax. Some support "line continuation", where a backslash followed immediately by EOL (end-of-line) causes the line break to be ignored, and the "logical line" to be continued on the next actual line from the INI file. Implementation of various "special characters" with escape sequences is also seen.[11]


## Common escape sequences

Sequence	Meaning
\\	\ (a single backslash, escaping the escape character)
\'	Apostrophe
\"	Double quotes
\0	Null character
\a	Bell/Alert/Audible
\b	Backspace, Bell character for some applications
\t	Tab character
\r	Carriage return
\n	Line feed
\;	Semicolon
\#	Number sign
\=	Equals sign
\:	Colon
\x????	Unicode character with hexadecimal code point corresponding to ????
