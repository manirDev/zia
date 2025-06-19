def define_env(env):
    """
    This is the hook for defining variables, macros and filters
    """
    
    # Add custom variables
    env.variables['project_name'] = 'Zia'
    
    # Add custom macros
    @env.macro
    def hello(name="World"):
        return f"Hello {name}!"
    
    # Add custom filters
    @env.filter
    def uppercase(text):
        return text.upper()