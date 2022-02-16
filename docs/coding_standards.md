# Coding Standards

## **Naming Conventions**

- Underscore with private attribute and method names
- Camel case for class names
- Snake case for attribute and method names

## **Code Related**
- assign operators > which one is better and why: 
      const RequestMessage& operator=(const RequestMessage& other);
      RequestMessage& operator=(const RequestMessage& other);

## **Other Rules**

**HPP Files**
- publics or privates are on top?
- ifndef or pragma?
- the order of including libraries? <vector> first then "ConfigParser.hpp"
- tabs or space before attribute/method name? also indentation?

## **Pushing and Merging**
- new branch for every feature (there is no long-living branches)
- main branch must be merged into the feature branch first, all merge conflicts are solved and then pull request is created
