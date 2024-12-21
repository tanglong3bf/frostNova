<script lang="ts" setup>
import { ref, defineEmits } from 'vue'
import { Expand, Fold, ArrowRight, CaretTop, CaretBottom } from '@element-plus/icons-vue'

// 和侧边栏菜单相关
const props = defineProps({
  asideCollapse: {
    type: Boolean,
    required: true,
    value: false,
  }
})

const emits = defineEmits(['toggleAsideCollapse'])
const toggleAsideCollapse = () => {
  emits('toggleAsideCollapse')
}

// 头像菜单相关
const menuCollapse = ref(false)
const toggleMenuCollapse = (val: boolean) => {
  menuCollapse.value = val
}

</script>

<template>
  <el-container>
    <el-button @click="toggleAsideCollapse" plain>
      <el-icon v-if="asideCollapse"><Fold /></el-icon>
      <el-icon v-else><Expand /></el-icon>
    </el-button>
    <el-breadcrumb :separator-icon="ArrowRight">
      <el-breadcrumb-item :to="{ path: '/' }">首页</el-breadcrumb-item>
      <el-breadcrumb-item>后台管理</el-breadcrumb-item>
      <el-breadcrumb-item>用户管理</el-breadcrumb-item>
    </el-breadcrumb>

    <el-dropdown
      placement="bottom-end"
      class="header-right"
      @click="toggleMenuCollapse"
      trigger="click"
      @visible-change="toggleMenuCollapse"
    >
      <div>
        <el-avatar shape="square" />
        <el-icon v-if="menuCollapse"><CaretBottom /></el-icon>
        <el-icon v-else><CaretTop /></el-icon>
      </div>
      <template #dropdown>
        <el-dropdown-menu>
          <el-dropdown-item>个人中心</el-dropdown-item>
          <el-dropdown-item>退出登录</el-dropdown-item>
        </el-dropdown-menu>
      </template>
    </el-dropdown>
  </el-container>
</template>

<style lang="scss" scoped>
.el-container {
  align-items: center;
  .el-breadcrumb {
     margin-left: 20px;
  }
  .header-right {
    margin-left: auto;
    cursor: pointer;
  }
}
</style>