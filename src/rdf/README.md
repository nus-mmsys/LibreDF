# RDF

## Application development

### RDF extension

```
rule <name> {

left {
    topology {
        ...
    }
    production {    
        ...
    }
    consumption {    
        ...
    }
    actor ... {
        ...
    }
}

right {
    topology {
        ...
    }
    production {    
        ...
    }
    consumption {    
        ...
    }
    actor ... {
        ...
    }
}

}
```

### Controller

A controller decides when and how to apply a transformation rule. It is not implemented yet. Controller can be implemented as follows.

```
main {
   <condition_1> : <rule_x>;
   ...
   <condition_n> : <rule_y>;
}
```


## Getting started

### Build

```
./build -DBUILD_RDF_MODULE=ON
```

### Run

```
./rdf ../test/rdf/cannyshow.df
```

### Commands 

```
graph               display the graph.
h                   display help menu.
latency             compute the lateny of the graph.
program             display the RDF program.
rules               display list of rules.
run                 run the RDF program.
schedule            compute an schedule for the graph.
```
