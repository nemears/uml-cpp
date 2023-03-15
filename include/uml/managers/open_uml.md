# Open-UML

This document is here to describe the open-uml protocol and serialization, it should be used as a guide for development as well as the canonical notation of how the serialization and protocol works.

The purpose of specifying a sperate serialization from OMG MOF XMI is that XMI is bulky, hard to read and is structured in an inefficient manner for platform and project management. This serialization along with the protocol is specified so that uml applications can have a simple, fast and common engine to tie into.

## Serialization
The serialization of uml objects the "Open-UML" way is usually in YAML or JSON. For this document we will detail in JSON. There are two modes of serialization used for different aspects of the uml codebase, Individual serialization and Whole serialization. Both can be serialized to files and serialized to strings for use within the codebase. The definitions of Individual and whole in regards to serialization of elements as follows:  

Individual refers to the serialization of a single uml element

Whole refers to the serialization of an entire uml project (collection of uml elements).

We will dive into the specific differences in these serializations.

This is seen in the use of the Serialization policy. A Serialization Policy must implement these following methods for a uml manager.  
- parseIndividualFile(path) -> ElementPtr : this method parses a file representing a singular element returning the element in an ElementPtr, or a null ElementPtr if it could not find it.
- parseIndividualString(string) -> ElementPtr : this method parses a string representing a singular element returning the element in an ElementPtr, or throwing an error  
- parseWholeFile(path) -> ElementPtr : this method parses a file representing an entire project of uml elements, it returns an element in an ElementPtr or a null ElementPtr if it could not parse the file
- parseWholeString(string) -> ElementPtr : this method parses a string representing an entire project of elements and will return an element in an elementptr or throw an error.

### basic serialization
The basics behind serialization is similar to breaking down our C++ objects into a json form. We require our objects to specify their type for open-uml specific serialization. They also need to display their id under that type, an empty Package object would be serialized to this:
```
{
    "package":
    {
        "id": "m3ixuPETCXTA2EgBYEEQP4nBGhoG"
    }
}
```
This serialization is the same within Individual and Whole serialization. The other things that stay the same are Primitive Type like properties e.g:
```
{
    "package":
    {
        "id": "m3ixuPETCXTA2EgBYEEQP4nBGhoG",
        "name": "model"
    }
}
```
and
```
{
    "literalInt":
    {
        "id": "932u3jub3IgVWjKMc8M4ok6jCmW4",
        value: 1
    }
}
```
What else is the same between the Individual and Whole serialization is any set that does subset or redefine ownedElements, or owner. The names of the methods within the c++ codebase are representative of the labels of sets / singletons of other elements. For example:
```
{
    "activityNode":
    {
        "id": "ApxSSoVRtJC8icnbzsT20lNwInID",
        "incomingEdges": [
            "F_TrPjt&DtpffafvcDnFaXH6ovd4"
        ]
    }
}
```
and for a singleton:
```
{
    "property":
    {
        "id": "QHvQsXZj4biIqXsETKaWWiDfNkz7",
        "type": "XI35viryLd5YduwnSbWpxSs3npcu"
    }
}
```
Now for all the other sets (all sets that subset ownedElements and singletons that subset owner) we specify them differently. In individual serialization we specify the owned elements subsets as the same, but adding a file extension to denote (this is subject to change because the file extension is not necessary for parsing) e.g:
```
{
    "package":
    {
        "id": "m3ixuPETCXTA2EgBYEEQP4nBGhoG",
        "packagedElements":
        [
            "zvAhfu&WNCI1cNXd3BkqhxywHAYY.yml"
        ]
    }
}
```
and an owner singleton would be displayed as such:
```
{
    "package":
    {
        "id": "zvAhfu&WNCI1cNXd3BkqhxywHAYY",
    },
    "owningPackage": "m3ixuPETCXTA2EgBYEEQP4nBGhoG"
}
```
For Whole style emitting, instead of the file extension we just put the elements themselves below, and then we omit the owner package declaration, so the prior two "Individual" serializations would translate to this as a "Whole" style:
```
{
    "package":
    {
        "id"  "m3ixuPETCXTA2EgBYEEQP4nBGhoG",
        "packagedElements":
        [
            {
                "package":
                {
                    "id": "zvAhfu&WNCI1cNXd3BkqhxywHAYY"
                }
            }
        ]
    }
}
```
So now it is up to the manager to also handle the data properly depending on the two styles. This may be discussed further if needed.

## Protocol

In regards of protocol we are mostly talking about the uml-server executable that is bundled up in the build of this library. It is a base version of a service(TODO) that can be used to drive the memory and persistence of uml sessions / projects. The protocol itself is established by communicating through tcp usually on port 8652 (UMLC). The uml-server holds reference to a singular project which usually entails one singular model which owns many projects. The server can work with objects besides model as the singular root of the server, this is refered to as the *root* within the rest of this document. 

To start off communication with the server a client application will have to send a uml ID string (28 character url safe base64 encoded string) after the server sends an initial ID request C-string: "id" (including the null terminating character '\0'). The server will then keep track of requests from that client along with other clients that it accepts connections from.  

Client requests follow a generally restful logic where the server will not actively send messages to them without a request from them first, usually following GET PUT POST and DELETE forms (but not included to just those). These requests will be detailed below. The requests are usually in the form of json (but yaml serialization can also be accepted).

### POST and post
A post request includes information of a uml id, and a uml element type e.g:  

```
{
    "POST": "PACKAGE",
    "id":  "m3ixuPETCXTA2EgBYEEQP4nBGhoG"
}
```
A post request makes a blank element of the type specified within the memory of the server.

### DELETE and delete
A delete request deletes data from the server, it is just specified with a singular id of an element to delete:
```
{
    "DELETE": "m3ixuPETCXTA2EgBYEEQP4nBGhoG"
}
```

### GET and get
A get request holds data of just the id the client wants to get e.g:
```
{
    "get": "m3ixuPETCXTA2EgBYEEQP4nBGhoG"
}
```
This request requires the server to respond with data of the element with the specified id in json/yaml format so something like this (see serialization above):
```
{
    "package" : 
    {
        "id": "m3ixuPETCXTA2EgBYEEQP4nBGhoG",
        "name": "Model",
        "packagedElements": 
        [
            "UML_r67OnwwyTHCtCmWnZsd8ePh5.yml"
        ]
    }
}
```

### PUT and put
A put request sends data for the server to hold / save in its memory / on disk. An example of a put request would be something along the lines of this:
```
{
    "put":
    {
        "id": "m3ixuPETCXTA2EgBYEEQP4nBGhoG",
        "element": 
        {
            "package" : 
            {
                "id": "m3ixuPETCXTA2EgBYEEQP4nBGhoG",
                "name": "Model",
                "packagedElements": 
                [
                    "UML_r67OnwwyTHCtCmWnZsd8ePh5.yml"
                ]
            }
        }
    }
}
```
This also makes use of the serialization specified later above.