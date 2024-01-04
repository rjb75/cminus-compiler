program
    func @ line 3
        type [void] @ line 3
        new_id [global_foo] @ line 3
        parameters
            var_param @ line 3
                type [int] @ line 3
                new_id [global_foo_a] @ line 3
            var_param @ line 3
                type [int] @ line 3
                new_id [global_foo_b] @ line 3
            var_param @ line 3
                type [int] @ line 3
                new_id [global_foo_c] @ line 3
        block
            declarations
            statements
                while @ line 5
                    < @ line 5
                        var [global_foo_a] @ line 5
                        var [global_foo_b] @ line 5
                    block
                        declarations
                            var_decl @ line 7
                                type [int] @ line 7
                                new_id [global_foo_while_c] @ line 7
                        statements
                            exprstmt @ line 8
                                = @ line 8
                                    var [global_foo_while_c] @ line 8
                                    / @ line 8
                                        var [global_foo_b] @ line 8
                                        var [global_foo_a] @ line 8
                            exprstmt @ line 9
                                = @ line 9
                                    var [a] @ line 9
                                    + @ line 9
                                        var [a] @ line 9
                                        var [c] @ line 9
                return @ line 11
    func @ line 14
        type [void] @ line 14
        new_id [global_bar] @ line 14
        parameters
            var_param @ line 14
                type [int] @ line 14
                new_id [global_bar_a] @ line 14
            var_param @ line 14
                type [int] @ line 14
                new_id [global_bar_b] @ line 14
        block
            declarations
            statements
                if_else @ line 16
                    < @ line 16
                        var [global_bar_a] @ line 16
                        var [global_bar_b] @ line 16
                    block
                        declarations
                            var_decl @ line 18
                                type [int] @ line 18
                                new_id [global_bar_if_a] @ line 18
                        statements
                            exprstmt @ line 19
                                = @ line 19
                                    var [global_bar_if_a] @ line 19
                                    int [3] @ line 19
                    block
                        declarations
                            var_decl @ line 22
                                type [int] @ line 22
                                new_id [global_bar_else_b] @ line 22
                        statements
                            exprstmt @ line 23
                                = @ line 23
                                    var [global_bar_else_b] @ line 23
                                    int [4] @ line 23
    func @ line 27
        type [void] @ line 27
        new_id [global_main] @ line 27
        parameters
        block
            declarations
            statements
                exprstmt @ line 29
                    funccall @ line 29
                        var [global_foo] @ line 29
                        arguments
                            int [2] @ line 29
                                int [7] @ line 29
                                    int [2] @ line 29
                return @ line 30
