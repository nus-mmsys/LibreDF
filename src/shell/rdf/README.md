# RDF

## Commands 

```
	benchmark		display benchmark.
	graph			display df graph.
	h			display help menu.
	rules			display list of rules.
	run			run the data graph.
```

## RDF extension

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



## Controller

A controller decides when and how to apply a transformation rule. It is not implemented yet. Controller can be implemented as follows.

```
main {
   <condition_1> ? <rule_x>;
   <condition_i> ? <rule_y> ; <rule_z>;
   ...
   <condition_n> ? <rule_t>*;
}
```
