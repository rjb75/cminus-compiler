program
    func @ line 5
        type [int] @ line 5
        new_id [gcd] @ line 5
        parameters
            var_param @ line 5
                type [int] @ line 5
                new_id [u] @ line 5
            var_param @ line 5
                type [int] @ line 5
                new_id [v] @ line 5
        block
            declarations
            statements
                if_else @ line 6
                    == @ line 6
                        var [v] @ line 6
                        int [0] @ line 6
                    return @ line 6
                        var [u] @ line 6
                    return @ line 7
                        funccall @ line 7
                            var [gcd] @ line 7
                            arguments
                                var [v] @ line 7
                                - @ line 7
                                    var [u] @ line 7
                                    * @ line 7
                                        / @ line 7
                                            var [u] @ line 7
                                            var [v] @ line 7
                                        var [v] @ line 7
    func @ line 11
        type [void] @ line 11
        new_id [main] @ line 11
        parameters
        block
            declarations
                var_decl @ line 12
                    type [int] @ line 12
                    new_id [x] @ line 12
                var_decl @ line 12
                    type [int] @ line 12
                    new_id [y] @ line 12
            statements
                exprstmt @ line 13
                    = @ line 13
                        var [x] @ line 13
                        funccall @ line 13
                            var [input] @ line 13
                            arguments
                exprstmt @ line 13
                    = @ line 13
                        var [y] @ line 13
                        funccall @ line 13
                            var [input] @ line 13
                            arguments
                exprstmt @ line 14
                    funccall @ line 14
                        var [output] @ line 14
                        arguments
                            funccall @ line 14
                                var [gcd] @ line 14
                                arguments
                                    var [x] @ line 14
                                    var [y] @ line 14
