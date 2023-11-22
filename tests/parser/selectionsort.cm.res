program
    array_decl @ line 4
        type [int] @ line 4
        new_id [x] @ line 4
        int [10] @ line 4
    func @ line 6
        type [int] @ line 6
        new_id [minloc] @ line 6
        parameters
            array_param @ line 6
                type [int] @ line 6
                new_id [a] @ line 6
            var_param @ line 6
                type [int] @ line 6
                new_id [low] @ line 6
            var_param @ line 6
                type [int] @ line 6
                new_id [high] @ line 6
        block
            declarations
                var_decl @ line 7
                    type [int] @ line 7
                    new_id [i] @ line 7
                var_decl @ line 7
                    type [int] @ line 7
                    new_id [x] @ line 7
                var_decl @ line 7
                    type [int] @ line 7
                    new_id [k] @ line 7
            statements
                exprstmt @ line 8
                    = @ line 8
                        var [k] @ line 8
                        var [low] @ line 8
                exprstmt @ line 9
                    = @ line 9
                        var [x] @ line 9
                        array_var [a] @ line 9
                            var [low] @ line 9
                exprstmt @ line 10
                    = @ line 10
                        var [i] @ line 10
                        + @ line 10
                            var [low] @ line 10
                            int [1] @ line 10
                while @ line 11
                    < @ line 11
                        var [i] @ line 11
                        var [high] @ line 11
                    block
                        declarations
                        statements
                            if @ line 12
                                < @ line 12
                                    array_var [a] @ line 12
                                        var [i] @ line 12
                                    var [x] @ line 12
                                block
                                    declarations
                                    statements
                                        exprstmt @ line 13
                                            = @ line 13
                                                var [x] @ line 13
                                                array_var [a] @ line 13
                                                    var [i] @ line 13
                                        exprstmt @ line 14
                                            = @ line 14
                                                var [k] @ line 14
                                                var [i] @ line 14
                            exprstmt @ line 15
                                = @ line 15
                                    var [i] @ line 15
                                    + @ line 15
                                        var [i] @ line 15
                                        int [1] @ line 15
                return @ line 17
                    var [k] @ line 17
